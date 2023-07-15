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
        this->path = rhs.path;
        this->method = rhs.method;
        this->fileName = rhs.fileName;
    }
    return *this;
}

CGI::~CGI() {
    if (envArray)
        delete[] envArray;
}

bool CGI::execCGI(Resources &resources, std::string path, std::ifstream &file) {
	std::cout << "CGI\n";
    std::cout << path << std::endl;
    size_t pos = path.find("/cgi/");

    this->path = path;
    if (pos != std::string::npos) {
        fileName = path.substr(pos + 5, path.find("?"));
        if (fileName.empty())
            return false;
        if (exec(resources, path))
            return true;
    }
    return false;
}

bool CGI::checkExtension() {
    size_t pos = path.find(".");

    if (pos == std::string::npos) {
        fileExtension = "";
        return false;
    } else if (path.substr(pos) == ".py")
        fileExtension = "py";
    else if (path.substr(pos) == ".php")
        fileExtension = "php";
    std::cout << "file extention: " << fileExtension << std::endl;
    return true;
}

void CGI::setEnv(Resources& resources) {
    if (fileExtension == "py")
        env["PROGRAM_NAME"] = "./CGI/python-cgi";
    else if (fileExtension == "php")
        env["PROGRAM_NAME"] = "./CGI/php-cgi";
    env["FILE_NAME"] = path;
    env["SCRIPT_NAME"] = path;
    env["SCRIPT_FILENAME"] = path;
    env["REDIRECT_STATUS"] = "200";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["REQUEST_METHOD"] = resources.getRequest("Method");
    env["CONTENT_LENGTH"] = resources.getRequest("ContentLength");
    env["CONTENT_TYPE"] = resources.getRequest("ContentType");
    env["PATH_INFO"] = path;
    env["QUERY_STRING"] = resources.getRequest("QueryString");
    env["SERVER_NAME"] = resources.getRequest("ServerName");
    env["SERVER_PORT"] = "8080";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "webserv";
    env["REMOTE_ADDR"] = "";
    env["HTTP_ACCEPT"] = resources.getRequest("Accept");
    env["HTTP_ACCEPT_ENCODING"] = resources.getRequest("AcceptEncoding");
    env["HTTP_ACCEPT_LANGUAGE"] = resources.getRequest("AcceptLanguage");
    env["HTTP_CONNECTION"] = resources.getRequest("Connection");
    env["HTTP_HOST"] = resources.getRequest("Host");
    env["HTTP_USER_AGENT"] = resources.getRequest("UserAgent");

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

bool CGI::exec(Resources& resources, std::string path) {
    std::string infile(path);
    
    outFile = "./CGI/outFile";

    int in_fd = open(path.c_str() , O_RDONLY);
	int out_fd = open(outFile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0755);

    checkExtension();setEnv(resources);getEnvAsArray();

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
            buff.clear();
            buff << outFileStr.substr(outFileStr.find("\r\n\r\n") + 4, outFileStr.size());
            outFileStr = buff.str();
            oFile = file;
            file.close();
            
            std::cout << "CGI EXECUTED\n";
            return true;
        }
    }
    kill(pid, SIGTERM);
    statusCode = "404";
    return false;
}

std::ifstream CGI::getOutFile() {
    return this->oFile;
}

size_t CGI::getOutFileSize() {
    return this->outFileStr.size();
}

std::string CGI::getStatusCode() {
    return this->statusCode;
}