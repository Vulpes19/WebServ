#include "CGI.hpp"

// check file name and extension
// set variabls
// exec file

CGI::CGI() {}

CGI &CGI::operator = (const CGI &rhs) {
    if (this != &rhs) {
        this->env = rhs.env;
        this->envArray = rhs.envArray;
        this->fileExtension = rhs.fileExtension;
        this->URI = rhs.URI;
        this->method = rhs.method;
        this->fileName = rhs.fileName;
    }
    return *this;
}

CGI::~CGI() {
    // if (envArray)
    //     delete[] envArray;
}

bool CGI::checkCGI(Resources &resources, std::string path) {
    //   /CGI/login.php?val1=val2    /CGI/session/login/login.php?val1=val2
	std::cout << "CGI\n";
    std::cout << URI << std::endl;

    URI = path;
    size_t pos = URI.find("/cgi-bin/"); // Compare to config path

    // check for directory in /cgi-bin/

    if (pos != std::string::npos && pos + 9 != std::string::npos) {
        if (URI.find("?") != std::string::npos) {
            fileName = URI.substr(pos + 9, URI.find("?"));
            std::pair<std::string, std::string> p("QUERY_STRING", URI.substr(URI.find("?") + 1, URI.length()));
            env.insert(p);
        }
        else fileName = URI.substr(pos + 9, URI.length());

        if (!fileName.empty() && checkExtension())
            setEnv(resources);
        else return false;
    }
    std::cout << "NOT CGI\n";
    return false;
}

bool CGI::checkExtension() {
    size_t pos = URI.find(".");

    if (pos == std::string::npos)
        return false;
    else if (URI.substr(pos, 3) == ".py")
        fileExtension = ".py";
    else if (URI.substr(pos, 4) == ".php")
        fileExtension = ".php";
    std::cout << "file extention: " << fileExtension << std::endl;
    return true;
}

void CGI::setEnv(Resources &resources) {
    if (fileExtension == ".py")
        env["PROGRAM_NAME"] = "./var/www/cgi-bin/python-cgi";
    else if (fileExtension == ".php")
        env["PROGRAM_NAME"] = "./var/www/cgi-bin/php-cgi";
    env["FILE_NAME"] = fileName;
    env["SCRIPT_NAME"] = fileName;
    env["SCRIPT_FILENAME"] = "/Users/elias/Desktop/cursus/webserv" + URI.substr(0, URI.length() - 1);
    env["REDIRECT_STATUS"] = "200";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["QUERY_STRING"] = "";
    env["SERVER_PORT"] = "80";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "webserv";
    env["HTTP_ACCEPT"] = resources.getRequest("Accept");
    env["HTTP_ACCEPT_ENCODING"] = "";
    env["HTTP_ACCEPT_LANGUAGE"] = "";
    env["HTTP_CONNECTION"] = resources.getRequest("Connection");
    env["HTTP_HOST"] = resources.getRequest("Host");
    env["HTTP_USER_AGENT"] = "";
    env["REQUEST_METHOD"] = resources.getRequest("Method");
    env["CONTENT_LENGTH"] = "";
    env["CONTENT_TYPE"] = "";
    env["SERVER_NAME"] = resources.getRequest("Host");
    env["REMOTE_ADDR"] = "";

    std::cout << "\n----------------------------------------------------------------" << std::endl;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    std::cout << "----------------------------------------------------------------" << std::endl;
}

void CGI::getEnvAsArray() {
    int i = 0;
    envArray = new char *[env.size() + 1];
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); ++it) {
        std::string tmp = it->first + "=" + it->second;
        envArray[i] = new char[tmp.length() + 1];
        strcpy(envArray[i], tmp.c_str());
        i++;
    }
    envArray[i] = NULL;
}

bool CGI::exec() {
    std::string infile(URI);
    
    outFile = "/Users/elias/Desktop/cursus/webserv/var/www/CGI/outFile";

    int in_fd = open(URI.c_str() , O_RDONLY);
	int out_fd = open(outFile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0755);

    getEnvAsArray();

    pid_t pid = fork();
    if (pid == -1)
        std::cerr << "Failed to fork" << std::endl;
    else if (pid == 0) {
        std::cout << "This is the child process" << std::endl;
        char *arg[] = {const_cast<char *>((*env.find("PROGRAM_NAME")).second.c_str()),const_cast<char *>((*env.find("FILE_NAME")).second.c_str()), NULL};

        dup2(in_fd, 0);
        dup2(out_fd, 1);
        execve(arg[0], arg, envArray);
        std::cerr << "Execve failed!" << std::endl;
        exit(1);
    }

    std::stringstream buff;
    std::ifstream file(outFile);

    int start_time = time(NULL);
	int timeout = 120;
	while (time(NULL) - start_time < timeout)
	{
        pid_t p = waitpid(pid, NULL, WNOHANG);
        if (p > 0) {
            std::cout << "Child process " << p << std::endl;
            close(in_fd);
            close(out_fd);
            buff << file.rdbuf();
			outFileStr = buff.str();
			if (outFileStr.size() == 0)
				statusCode = "400";
			if (outFileStr.substr(0, 7) == "Status:")
				statusCode = outFileStr.substr(outFile.find("Status:") + 9, 3);
            std::ofstream oFile(outFile);
            if (oFile.is_open())
                oFile << "HTTP/1.1 200 Ok\r\nContent-Length: " << outFileStr.size() << "\r\n" << outFileStr;
            std::cout << "content size " << std::to_string((outFileStr.substr(outFileStr.find("\r\n\r\n") + 4, outFileStr.size())).length()) << std::endl;
            file.close();
            oFile.close();
            std::cout << "CGI EXECUTED\n";
            return true;
        }
    }
    kill(pid, SIGTERM);
    statusCode = "404";
    return false;
}

size_t CGI::getOutFileSize() {
    return this->outFileStr.size();
}

std::string CGI::getStatusCode() {
    return this->statusCode;
}

void CGI::setURI(std::string path) {
    this->URI = path;
}