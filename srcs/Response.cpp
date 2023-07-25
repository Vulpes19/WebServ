/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:08 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/25 20:57:48 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void	ErrorResponse::normalizePath( std::string &path, enum ERR err)
{
	try
	{
		if ( err == ACCESS )
		{
			if (path[0] == '/')
				path.erase(0);
			if ( path[path.length() - 1] == '/' )
				path.erase(path.size() - 1);
		}
		else
		{
			if ( path[0] != '/' )
				path = "./" + path;
			if ( path[0] == '/' )
				path = "." + path;
			if ( path[path.length() - 1] == '/' )
				path.erase(path.size() - 1);
		}
		
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERRROOOOOOR" << '\n';
	}
	
}

void    ErrorResponse::errorBadRequest( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 400 Bad Request\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 11 << "\r\n\r\n";
		errorMsg << "Bad Request";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorNotFound( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	// size_t pos = path.find("assets");
	// path = path.substr(pos);
	errorMsg << "HTTP/1.1 404\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 9 << "\r\n\r\n";
		errorMsg << "Not Found";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorForbidden( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 403 Forbidden\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 9 << "\r\n\r\n";
		errorMsg << "Forbidden";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorInternal( SOCKET socket, std::string path )
{
	std::cout << path << std::endl;
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 500 Internal Server Error\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	if ( !path.empty() )
		normalizePath(path, ACCESS);
	std::cout << "PATH: " << path << std::endl;
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 21 << "\r\n\r\n";
		errorMsg << "internal server error";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	std::cout << errorMsg.str() << std::endl;
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorUnauthorized( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 401 Unauthorized\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 12 << "\r\n\r\n";
		errorMsg << "Unauthorized";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorMethodNotAllowed( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 405 Method Not Allowed\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 18 << "\r\n\r\n";
		errorMsg << "Method Not Allowed";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorLengthRequired( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 411 Length Required\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 15 << "\r\n\r\n";
		errorMsg << "Length Required";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorHTTPVersion( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 505 HTTP Version Not Supported\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 26 << "\r\n\r\n";
		errorMsg << "HTTP Version Not Supported";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

void    ErrorResponse::errorRequestTooLarge( SOCKET socket, std::string path )
{
	std::stringstream errorMsg;
	errorMsg << "HTTP/1.1 413 Request Entity Too Large\r\n";
	errorMsg << "Connection: close\r\n";
	errorMsg << "Content-Type: text/html\r\n";
	normalizePath(path, ACCESS);
	if ( access(path.c_str(), F_OK | R_OK) == -1 )
	{
		errorMsg << "Content-Length: " << 25 << "\r\n\r\n";
		errorMsg << "Request Entity Too Large";
	}
	else
	{
		normalizePath(path, EXEC);
		errorMsg <<	"Content-Length: " << help.getFileSize(path.c_str()) << "\r\n\r\n";
		std::ifstream file(path.c_str(), std::ios::binary);
		if ( !file.is_open() )
			return ;
		std::string line;
		while ( std::getline(file, line) )
		{
			if ( !file.eof() )
				line += "\n";
			errorMsg << line;
		}
	}
	errorMsg << "\r\n";
	if ( send( socket, errorMsg.str().data(), errorMsg.str().size(), 0 ) == -1 )
		errorInternal(socket, "/assets/error_images/500.html");
}

Response::Response( void )
{
	autoIndex = false;
	bytesReceived = 0;
	bytesSent = 0;
	fileSize = 0;
	socket = -1;
	indexResponse = "";
	bodySize = 0;
	isBody = false;
	uploadPath = "NONE";
	cgi.isCGI = false;
	cgi.statusCode = -1;
}

Response::~Response( void )
{
	loc.clear();
	errorPages.clear();
}

void    Response::setSocket( SOCKET socket )
{
	this->socket = socket;
}

enum ResponseStates    Response::handleReadRequest( Resources &resources, std::string &name )
{
	char    request[4096];
	ssize_t bytesRead;
	size_t	lenPos;
	size_t	hostPos;
	size_t	endPos;
	size_t	delimiter;
	
	if ( !buffer.is_open() )
		buffer.open("readingRequestFile", std::ios::binary);
	bytesRead = read( socket, request, 4096 );
	if ( bytesRead > 0  )
	{
		std::string toCheck(request, bytesRead); 
		// std::cout << toCheck << std::endl;
		buffer.write(request, bytesRead);
		lenPos = toCheck.find("Content-Length: ");
		hostPos = toCheck.find("Host: ");
		if ( hostPos != std::string::npos )
		{
			hostPos += 6;
			endPos = toCheck.find("\r\n", hostPos);
			if ( endPos != std::string::npos )
			{
				std::string hostStr = toCheck.substr(hostPos, endPos - hostPos );
				if ( !hostStr.empty() )
				{
					if ( hostStr != host + ":" + port && hostStr != host )
						name = hostStr;
				}
			}
		}
		if ( lenPos != std::string::npos )
		{
			lenPos += 16;
			endPos = toCheck.find("\r\n", lenPos);
			if ( endPos != std::string::npos )
			{
				std::string lenStr = toCheck.substr(lenPos, endPos - lenPos);
				if ( !lenStr.empty() )
				{
					bodySize = atol(lenStr.c_str());
					if ( bodySize > bodyLimit )
					{
						err.errorRequestTooLarge(socket, errorPages["413"]);
						buffer.close();
						if ( remove("readingRequestFile") != 0 )
							err.errorInternal(socket, errorPages["500"]); 
						reset();
						return (RESET);
					}
				}
			}
		}
		std::string end("\r\n\r\n");
		delimiter = toCheck.find(end);
		if ( isBody )
			bytesReceived += bytesRead;
		if ( delimiter != std::string::npos )
		{
			isBody = true;
			std::string bodyStart(toCheck.begin() + delimiter + end.length(), toCheck.end());
			if ( !bodyStart.empty() )
				bytesReceived += bodyStart.length();
		}
		if ( bytesReceived >= bodySize )
		{
			buffer.close();
			resources.checkRequest();
			return (READY_TO_WRITE);	
		}
		return (READING);
	}
	else if ( bytesRead == 0 )
	{
		buffer.close();
		resources.checkRequest();
		return (READY_TO_WRITE);
	}
	else
	{
		buffer.close();
		err.errorBadRequest(socket, errorPages["400"]);
		if ( remove("readingRequestFile") != 0 )
			err.errorInternal(socket, errorPages["500"]);
		reset();
		return (RESET);
	}
}

bool	ResponseHelper::isDirectory( std::string path ) const
{
	struct stat fileInfo;

	if ( stat(path.c_str() , &fileInfo) == 0 )
		return S_ISDIR(fileInfo.st_mode);
	return (false);
}

enum ResponseStates    Response::getResponseDir( std::string path )
{
	std::ostringstream oss;
	std::string fullPath = "." + path;
	if ( !autoIndex )
	{
		err.errorForbidden(socket, errorPages["403"]);
		reset();
		return (RESET);
	}
	if ( access(fullPath.c_str(), F_OK) == -1 )
	{
		err.errorNotFound(socket, errorPages["404"]);
		reset();
		return (RESET);
	}
	if ( access(fullPath.c_str(), R_OK | X_OK) == -1 )
	{
		err.errorForbidden(socket, errorPages["403"]);
		reset();
		return (RESET);
	}
	DIR *dir = opendir(fullPath.c_str());
	if ( dir == NULL )
	{
		err.errorInternal(socket, errorPages["500"]);
		reset();
		return (RESET);
	}
	indexResponse += "<html><body><ul>";
	struct  dirent *entry;	
	while ( (entry = readdir(dir)) != NULL )
	{
		std::string name = entry->d_name;
		if ( name != "." && name != ".." )
		{
			if ( help.isDirectory( "./" + name ) )
				indexResponse += "<li><a href=\"" + path + "/" + name + "/\">" + name + "/</a></li>";
			else
				indexResponse += "<li><a href=\"" + path + "/" + name + "\">" + name + "</a></li>";
		}
	}
	indexResponse += "</ul></body></html>";
	oss << "HTTP/1.1 200 OK\r\n";
	oss << "Connection: close\r\n";
	oss << "Content-Length: " << indexResponse.size() << "\r\n";
	oss << "Content-Type: " << "text/html" << "\r\n";
	oss << "\r\n";
	if ( send(socket, oss.str().data(), oss.str().size(), 0) == -1 )
		err.errorInternal(socket, errorPages["500"]);
	if ( send(socket, indexResponse.data(), indexResponse.size(), 0) == -1 )
		err.errorInternal(socket, errorPages["500"]);
	if ( closedir(dir) == -1 )
		err.errorInternal(socket, errorPages["500"]);
	reset();
	return (RESET);
}

enum ResponseStates    Response::getResponseFile( Resources &resources, std::string path )
{
	(void)resources;
	if ( !file.is_open() )
	{
		std::ostringstream oss;
		bytesSent = 0;
		bytesReceived = 0;
		if ( path != "NONE" )
			oss << "." << path;
		else
		{
			err.errorForbidden(socket, errorPages["403"]);
			reset();
			return (RESET);
		}
		std::string fullPath = oss.str();
		if ( fullPath[fullPath.length() - 1] == '/' )
			fullPath = fullPath.substr(0, fullPath.length() - 1 );
		if ( access(fullPath.c_str(), F_OK) == -1 )
		{
			fullPath = fullPath.substr(1);
			if ( access(fullPath.c_str(), F_OK) == -1 )
			{
				err.errorNotFound(socket, errorPages["404"]);
				reset();
				return (RESET);
			}
		}
		if ( access( fullPath.c_str(), R_OK) == -1 )
		{
			err.errorForbidden(socket, errorPages["403"]);
			reset();
			return (RESET);
		}
		fileSize = help.getFileSize(fullPath.c_str());
		if ( fileSize > bodyLimit )
		{
			err.errorRequestTooLarge(socket, errorPages["413"]);
			reset();
			return (RESET);
		}
		file.open(fullPath.c_str());
		if ( !file.is_open() )
		{
			err.errorInternal(socket, errorPages["500"]);
			reset();
			return (RESET);
		}
		if ( sendResponseHeader( GET, "200 OK", fullPath, NULL ) == false )
		{
			reset();
			return (RESET);
		}
	}
	char buffer[BSIZE + 1];
	ssize_t bytesRead;
	file.read(buffer, BSIZE);
	bytesRead = file.gcount();
	buffer[bytesRead] = '\0';
	if ( bytesRead == -1 )
	{
		bytesSent = 0;
		file.close();
		reset();
		return (RESET);
	}
	else if ( bytesRead > 0 )
	{
		bytesSent = send( socket, buffer, bytesRead, 0 );
		if ( bytesSent == 0 || bytesSent == -1 )
		{
			reset();
			return (RESET);
		}
		bytesSent += bytesRead;
		if ( bytesSent == fileSize )
			return (RESET);
		return (READING);
	}
	else
	{
		bytesSent = 0;
		file.close();
		reset();
		return (RESET);
	}
}

enum ResponseStates	Response::postUploadFile( Resources &resources )
{
	std::string filePath(resources.getRequest("URL"));
	if ( filePath[filePath.length() - 1] == '/' )
		filePath = filePath.substr(0, filePath.length() - 1);
	if ( filePath.find_last_of("/") != std::string::npos )
		filePath = filePath.substr(filePath.find_last_of("/"));
	std::string type = help.getFileType(resources.getRequest("Content-Type"), TYPE_SUFFIX);
	if ( uploadPath != "NONE" )
		help.normalizePath(uploadPath);
	uploadPath += filePath.substr(1).c_str() + type;
	uploadPath = "." + uploadPath;
	std::cout << uploadPath << std::endl;
	if ( rename("requestBody", uploadPath.c_str()) != 0 )
	{
		err.errorInternal(socket, errorPages["500"]);
		reset();
		return (RESET);
	}
	if ( sendResponseHeader( POST, "201 Created", filePath, &resources ) == false )
	{
		reset();
		return (RESET);
	}
	if ( send( socket, "File created.", 13, 0 ) )
		err.errorInternal(socket, errorPages["500"]);
	reset();
	return (RESET);
}

bool	Response::checkCGI( std::string path )
{
	if ( path.find(".") != std::string::npos && path.find("cgi-bin") != std::string::npos )
		return (true);
	else
		return (false);
}

enum ResponseStates	Response::handleCGI( Resources &resources, std::string path )
{
	std::string queryStr = "";
	std::map< std::string, std::string > env;
	size_t pos = path.find("?");
	if ( pos != std::string::npos )
	{
		queryStr = path.substr(pos + 1);
		if ( queryStr[queryStr.length() - 1] == '/' )
			queryStr.erase(queryStr.length() - 1);
	}
	// exit(1);
	if ( resources.getRequest("Content-Length") != "NOT FOUND" )
		env["CONTENT_LENGTH"] = resources.getRequest("Content-Length");
	if ( resources.getRequest("Content-Type") != "NOT FOUND" )
		env["CONTENT_TYPE"] = resources.getRequest("Content-Type");
	env["REQUEST_METHOD"] = resources.getRequest("Method");
	if ( !queryStr.empty() )
		env["QUERY_STRING"] = queryStr;
	if ( !queryStr.empty() )
		path = path.substr(1, path.find("?") );
	if ( path[path.length() - 1] == '/' )
		path.erase(path.length() - 1);
	env["SCRIPT_NAME"] = path.substr(1);
	env["SCRIPT_FILENAME"] = path.substr(1);
	env["SERVER_NAME"] = serverName;
	env["SERVER_PORT"] = port;
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "webserv";
	env["REDIRECT_STATUS"] = "200";
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["PATH_INFO"] = path;
	env["PATH_TRANSLATED"] = path;
	env["REMOTE_ADDR"] = host;
	std::map<std::string, std::string> header = resources.getHeader();
	std::map<std::string, std::string>::iterator it = header.begin();
	for ( ; it != header.end(); ++it )
	{
		std::string key = it->first;
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		std::replace(key.begin(), key.end(), '-', '_');
		env["HTTP_" + key] = it->second;
	}
	if ( !executeCGI( path, env ) )
	{
		remove("output");
		return (RESET);
	}
	fileSize = help.getFileSize("output");
	cgi.file.open("output");
	if ( !cgi.file.is_open() )
		return (RESET);
	cgi.isCGI = true;
	return (READING);
}

char *const*Response::getEnvArr( std::map<std::string, std::string> &env )
{
	char **envp = new char *[env.size() + 1]; 
	std::map<std::string, std::string>::iterator it = env.begin();
	size_t i = 0;
	while ( i < env.size() && it != env.end() )
	{
		std::string envStr = it->first + "=" + it->second;
		envp[i] = new char[envStr.length() + 1];
		strcpy(envp[i], envStr.c_str());
		++it;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

bool	Response::executeCGI( std::string &path, std::map<std::string, std::string> &env )
{
	FILE	*outputFile = fopen("output", "w");
	if ( !outputFile )
		return (false);
	int fd = fileno(outputFile);
	int status;
	pid_t pid = fork();
	if ( pid == -1 )
		return (false);
	if ( pid == 0 )
	{
		dup2(fd, 1);
		close(fd);
		std::map<std::string, std::string>::iterator it = env.begin();
		for ( ; it != env.end(); ++it )
			setenv(it->first.c_str(), it->second.c_str(), 1);
		char *const *envp = getEnvArr(env);
		std::string cgi2 = cgi.cgiPath;
		if ( cgi.cgiPath.find("usr") == std::string::npos)
		{
			cgi.cgiPath = "." + cgi.cgiPath;
		}
		path = "." + path;
		if ( path.find("?") != std::string::npos )
			path = path.substr(0, path.find("?"));
		if ( path[path.length() - 1] == '/' )
			path = path.substr(0, path.length() - 1);
		std::vector<const char *> args;
		if ( path.find(".cgi") != std::string::npos )
		{
			cgi.cgiPath = path;
			args.push_back(path.c_str());
		}
		else
		{
			args.push_back(cgi.cgiPath.c_str());
			args.push_back(path.c_str());
		}
		args.push_back(NULL);
		execve(cgi.cgiPath.c_str(), const_cast<char *const *>(args.data()), envp);
		std::cerr << "execve failed\n";
		exit(127);
	}
	else
	{
		close(fd);
		int timeout = 5;
		int res;
		while ( timeout > 0 )
		{
			res = waitpid(pid, &status, WNOHANG);
			if ( res == pid )
			{
				if ( WIFEXITED(status) )
				{
					if ( WEXITSTATUS(status) == 127 )
						return (false);
					else
						return (true);
				}
				break ;
			}
			sleep(1);
			timeout--;
		}
		if ( res == 0 )
		{
			kill(pid, SIGTERM);
			waitpid(pid, &status, 0);
		}
	}
	close(fd);
	return (false);
}

bool	Response::sendCGI( void )
{
	std::string line;
	if ( std::getline(cgi.file, line) )
	{
		if ( line.find("error") != std::string::npos )
		{
			err.errorInternal(socket, errorPages["500"]);
			if ( remove("output") != 0 )
				err.errorInternal(socket, errorPages["500"]);
			reset();
			return (false);
		}
		if ( line.find("Powered-By") != std::string::npos )
			return (true);
		if ( line.find("status") != std::string::npos )
		{
			line.erase(line.length() - 1);
			line.replace(0, line.find("status") + 7, "HTTP/1.1");
			line += " OK\r";
		}
		if ( !cgi.file.eof() )
			line += '\n';
		ssize_t bytesSent = send( socket, line.data(), line.size(), 0 );
		if ( bytesSent == -1 )
		{
			err.errorInternal(socket, errorPages["500"]);
			remove("output");
			reset();
			return (false);
		}
		if ( bytesSent == 0 )
		{
			reset();
			return (false);
		}
		return (true);
	}
	else
	{
		cgi.file.close();
		if ( remove("output") != 0 )
			err.errorInternal(socket, errorPages["500"]);
		reset();
		return (false);
	}
}

enum ResponseStates	Response::deleteFile( std::string filePath, Resources &resources )
{
	if ( std::count(filePath.begin(), filePath.end(), '/') > 1 )
		filePath = "." + filePath;
	if ( filePath[filePath.length() - 1] == '/' )
		filePath = filePath.substr(0, filePath.length() - 1);
	if ( access(filePath.c_str(), F_OK) == -1 )
	{
		err.errorNotFound(socket, errorPages["404"]);
		reset();
		return (RESET);
	}
	if ( access(filePath.c_str(), W_OK) == -1 )
	{
		err.errorForbidden(socket, errorPages["403"]);
		reset();
		return (RESET);
	}
	if ( remove(filePath.c_str()) == 0 )
	{
		if ( sendResponseHeader( DELETE, "200 OK", "", &resources ) == false )
		{
			reset();
			return (RESET);
		}
	}
	else
		err.errorInternal(socket, errorPages["500"]);
	reset();
	return (RESET);
}
std::string Response::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}
bool    Response::handleWriteResponse( Resources &resources )
{
	enum ResponseStates ret;
	redir red = help.checkForRedirections(loc, resources.getRequest("URL"));
	std::string method = resources.getRequest("Method");
	std::cout << resources.getRequest("URL") << std::endl;
	if ( method == "NOT FOUND" )
	{
		err.errorMethodNotAllowed(socket, errorPages["405"]);
		resources.clear();
		return (true);
	}
	if ( red.status_code != "-1" )
	{
		if ( sendResponseHeader( REDIR, red.status_code, red.path, &resources) == false )
		{
			resources.clear();
			return (true);
		}
		resources.clear();
		return (true);
	}
	autoIndex = help.getAutoIndex(loc, resources.getRequest("URL"));
	if ( uploadPath == "NONE" )
		uploadPath = getUploadPath(resources.getRequest("URL"));
	// std::cout << uploadPath << std::endl;
	if ( resources.getError() != NO_ERROR )
	{
		if ( handleErrors( resources ) )
		{
			resources.clear();
			return (true);
		}
	}
	// std::cout << "URL " << resources.getRequest("URL") << std::endl; 
	std::string path = getRootPath(resources.getRequest("URL"));
	bool isAllowed = false;
	if ( allowedMethods.empty() )
		isAllowed = true;
	for ( size_t i = 0; i < allowedMethods.size(); i++ )
	{
		if ( method == allowedMethods[i] )
		{
			isAllowed = true;
			break ;
		}
	}
	if ( isAllowed == false && path.find("error") == std::string::npos )
	{
		err.errorMethodNotAllowed(socket, errorPages["405"]);
		resources.clear();
		return (true);
	}
	if ( checkCGI(path) && cgi.isCGI == false )
	{
		ret = handleCGI(resources, path);
		if ( ret == RESET)
		{
			err.errorBadRequest(socket, errorPages["500"]);
			resources.clear();
			reset();
			return(true);
		}
		else
			return (false);
	}
	if ( cgi.isCGI == true )
	{
		if ( sendCGI() )
			return (false);
		else
		{
			cgi.isCGI = false;
			cgi.file.close();
			if ( remove("output") != 0 )
				err.errorInternal(socket, errorPages["500"]);
			reset();
			return (true);
		}
	}
	// std::cout << "before " << path << std::endl;
	help.normalizePath(path);
	std::cout << "after " << path << std::endl;
	if ( path.find("..") != std::string::npos )
	{
		err.errorUnauthorized(socket, errorPages["401"]);
		resources.clear();
		return (true);
	}
	if ( method == "GET")
	{
		if ( help.isDirectory("." + path) && path != "/" )
			ret = getResponseDir(path);
		else
			ret = getResponseFile(resources, path);
	}
	if ( method == "POST"  )
	{
		if ( help.isDirectory("." + path) )
			err.errorForbidden(socket, errorPages["403"]);
		else
			ret = postUploadFile(resources);
	}
	if ( method == "DELETE" )
	{
		if ( help.isDirectory("." + path) )
			err.errorForbidden(socket, errorPages["403"]);
		else
			ret = deleteFile(path, resources);
	}
	if ( ret == READING )
		return (false);
	else
	{
		resources.clear();
		return (true);
	}
}

bool	Response::handleErrors( Resources &resources )
{
	switch ( resources.getError() )
	{
		case BAD_REQUEST:
			err.errorBadRequest(socket, errorPages["400"]);
			break ;
		case NOT_FOUND:
			err.errorNotFound(socket, errorPages["404"]);
			break ;
		case FORBIDDEN:
			err.errorForbidden(socket, errorPages["403"]);
			break ;
		case INTERNAL_SERVER_ERROR:
		{
			err.errorInternal(socket, errorPages["500"]);
			break ;
		}
		case UNAUTHORIZED:
			err.errorUnauthorized(socket, errorPages["401"]);
			break ;
		case METHOD_NOT_ALLOWED:
		{
			exit(1);
			err.errorMethodNotAllowed(socket, errorPages["405"]);
			break ;
		}
		case LENGTH_REQUIRED:
			err.errorLengthRequired(socket, errorPages["411"]);
			break ;
		case HTTP_VERSION_NOT_SUPPORTED:
			err.errorHTTPVersion(socket, errorPages["505"]);
			break ;
		case REQUEST_ENTITY_TOO_LARGE:
			err.errorRequestTooLarge(socket, errorPages["413"]);
			break ;
		default:
			return (false);
	}
	return (false);
}

bool	Response::sendResponseHeader( enum METHODS method, std::string statusCode, std::string fileName, Resources *resources )
{
	std::ostringstream oss;
	switch (method)
	{
		case GET:
		{
			oss << "HTTP/1.1 " << statusCode << "\r\n";
			oss << "Connection: close\r\n";
			oss << "Date: " << help.getCurrentTime() << "\r\n";
			oss << "Server: " << serverName << "\r\n";
			oss << "Content-Type: " << help.getFileType(fileName, TYPE_NAME) << "\r\n";
			oss << "Content-Length: "<< fileSize << "\r\n";
			break ;
		}
		case POST:
		{
			oss << "HTTP/1.1 " << statusCode << "\r\n";
			oss << "Connection: close\r\n";
			oss << "Date: " << help.getCurrentTime() << "\r\n";
			oss << "Server: " << serverName << "\r\n";
			std::string ret;
			ret = resources->getRequest("Content-Type");
			// if ( ret != "NOT FOUND" )
				// oss << "Content-Type: " << ret << "\n";
			// oss << "Content-Length: " << "13" << "\r\n";
			break ;
		}
		case DELETE:
		{
			oss << "HTTP/1.1 " << statusCode << "\r\n";
			oss << "Connection: close\r\n";
			oss << "Date: " << help.getCurrentTime() << "\r\n";
			oss << "Server: " << serverName << "\r\n";
			oss << "Content-Type: text/plain\r\n";
			oss << "Content-Length: 13\r\n\r\n";
			oss << "File deleted.\r\n";
			break ;
		}
		case REDIR:
		{
			oss << "HTTP/1.1 " << statusCode << "\r\n";
			oss << "Connection: close\r\n";
			oss << "Date: " << help.getCurrentTime() << "\r\n";
			oss << "Server: " << serverName << "\r\n";	
			if ( statusCode == "301" || statusCode == "302" )
				oss << "Location: " << fileName << "\r\n";
			else
			{
				oss << "Content-Type: text/html\r\n";
				oss << "Content-Length: " << fileName.size() << "\r\n";
				oss << "\r\n";
				oss << fileName.data() << "\r\n";
			}
			break ;
		}
		default:
			err.errorInternal(socket, errorPages["500"]);
	}
	oss << "\r\n";
	ssize_t ret = send( socket, oss.str().data(), oss.str().size(), 0 );
	if ( ret == -1 )
	{
		err.errorInternal(socket, errorPages["500"]);
		return (false);
	}
	if ( ret == 0 )
		return (false);
	return (true);
}

void    Response::setLocations( std::vector<Location> loc )
{
    this->loc = loc;
}

void	Response::setErrorPages( std::map< std::string, std::string > errorPages )
{
	this->errorPages = errorPages;
}
		
void	Response::setName( std::string name )
{
	this->serverName = name;
}

void	Response::setHost( std::string host )
{
	this->host = host;
}

void	Response::setPort( std::string port )
{
	this->port = port;
}

void	Response::setBodySize( ssize_t bodyLimit )
{
	this->bodyLimit = bodyLimit;
}

void	Response::setUpload( std::string uploadPath )
{
	this->uploadPath = uploadPath;
}

std::string	Response::getRootPath( std::string path )
{
	if ( path.empty() )
		path = "/";
	if ( path[path.length() - 1] != '/' )
		path += '/';
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( path != "/" && loc[i].getRoot().find("path") != std::string::npos )
		{
			allowedMethods = loc[i].getAllowedMethods();
			return (path);
		}
		if ( path == "/" && loc[i].getValue() == "/" )
		{
			std::cout << "Im here: " << loc[i].getValue() << std::endl;
			// if (allowedMethods.empty())
				allowedMethods = loc[i].getAllowedMethods();
			std::cout << "Im here " << loc[i].getValue() << std::endl;
			std::string rootPath = loc[i].getRoot();
			if ( path.find(rootPath) != std::string::npos )
				return (path);
			if ( rootPath[rootPath.length() - 1] != '/' )
				rootPath += '/';
			std::string indexFile = loc[i].getIndex();
			std::string ret = rootPath + path.substr(loc[i].getValue().length());
			if ( indexFile.empty() )
				return (ret);
			else
			{
				if ( loc[i].getValue().length() != path.length() )
					return ( ret );
				else
					return ( ret + indexFile );
			}
		}
		if ( loc[i].getValue().find("cgi-bin") != std::string::npos )
			cgi.cgiPath = loc[i].getCGI();
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
		{
			std::cout << "Im here: " << loc[i].getValue() << std::endl;
			if (allowedMethods.empty())
				allowedMethods = loc[i].getAllowedMethods();
			std::string rootPath = loc[i].getRoot();
			if ( path.find(rootPath) != std::string::npos )
				return (path);
			if ( rootPath[rootPath.length() - 1] != '/' )
				rootPath += '/';
			std::string indexFile = loc[i].getIndex();
			std::string ret = rootPath + path.substr(loc[i].getValue().length());
			if ( indexFile.empty() )
				return (ret);
			else
			{
				if ( loc[i].getValue().length() != path.length() )
					return ( ret );
				else
					return ( ret + indexFile );
			}
		}
	}
	return (path);
}

std::string	Response::getUploadPath( std::string path )
{
	if ( path.empty() )
		path = "/";
	if ( path[path.length() - 1] != '/' )
		path += '/';
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( (path == "/" || path.find("/") != std::string::npos) && loc[i].getValue() == "/" )
			return (loc[i].getUpload());
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
			return (loc[i].getUpload());
	}
	return ("NONE");
}

void	Response::reset( void )
{
	bytesReceived = 0;
	bytesSent = 0;
	fileSize = 0;
	socket = -1;
	indexResponse.clear();
	loc.clear();
	errorPages.clear();
	cgi.isCGI = false;
	cgi.statusCode = -1;
}

ssize_t  ResponseHelper::getFileSize( const char *path ) const
{
    struct stat fileStat;
    if ( stat(path, &fileStat) == 0 )
        return (fileStat.st_size);
    return (-1);
}

const std::string  	ResponseHelper::getFileType( std::string path, enum TYPES type ) const
{
    static std::map< std::string, std::string > fileTypes;
	if ( fileTypes.empty() )
	{
		fileTypes[".css"] = "text/css";
		fileTypes[".mp4"] = "video/mp4";
		fileTypes[".mp3"] = "audio/mpeg";
		fileTypes[".png"] = "image/png";
		fileTypes[".zip"] = "application/zip";
		fileTypes[".csv"] = "text/csv";
		fileTypes[".gif"] = "image/gif";
		fileTypes[".htm"] = "text/html";
		fileTypes[".html"] = "text/html";
		fileTypes[".ico"] = "image/x-icon";
		fileTypes[".jpeg"] = "image/jpeg";
		fileTypes[".jpg"] = "image/jpeg";
		fileTypes[".js"] = "application/javascript";
		fileTypes[".json"] = "application/json}";
		fileTypes[".pdf"] = "application/pdf";
		fileTypes[".svg"] = "image/svg+xml";
		fileTypes[".txt"] = "text/plain";
	}
	if ( type == TYPE_NAME )
	{
		size_t pos = path.find_last_of('.');
		std::string fileName = path;
		if ( pos != std::string::npos)
			fileName = path.substr(pos);
		std::map< std::string, std::string >::iterator it = fileTypes.find(fileName);
		if ( it != fileTypes.end() )
			return ( it->second );
		else
			return ("text/plain");
	}
	else
	{
		std::map< std::string, std::string >::iterator it = fileTypes.begin();
		for ( ; it != fileTypes.end(); ++it )
		{
			if ( it->second == path.substr(0, path.length() - 1) )
				return (it->first);
		}
		return ("text/plain");
	}
}

const std::string	ResponseHelper::getCurrentTime( void ) const
{
    std::time_t now = std::time(NULL);
    std::string dateTime = std::ctime(&now);
    dateTime.erase(dateTime.length() - 1);
    return (dateTime);
}

const std::string	ResponseHelper::getFileLocation( const char *relativePath ) const
{
	char path[PATH_MAX];

	char *absolutePath = realpath( relativePath, path );
	if ( absolutePath != NULL)
		return (absolutePath);
	else
		return ("");
}

void	ResponseHelper::normalizePath( std::string &path )
{
	bool prevWasSep = false;

	for ( size_t i = 0; i < path.length(); i++ )
	{
		if ( path[i] == '/' )
		{
			if ( prevWasSep )
			{
				path.erase(i, 1);
				i--;
			}
			else
				prevWasSep = true;
		}
		else
			prevWasSep = false;		
	}
	while ( !path.empty() && path[path.length() - 1] == '/' )
		path.erase(path.size() - 1);
	if ( !path.empty() && path[0] != '/' )
		path = '/' + path;
	if ( !path.empty() && path[path.length() - 1] != '/' )
		path += '/';
}

bool	ResponseHelper::getAutoIndex( std::vector<Location> &loc, std::string path ) const
{
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( path == "/" && loc[i].getValue() == "/" )
			return (loc[i].getAutoIndex());
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
			return (loc[i].getAutoIndex());
	}
	return (false);
}

const redir	ResponseHelper::checkForRedirections( std::vector<Location> &loc, std::string path ) const
{
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( path == "/" && loc[i].getValue() == "/" )
			return (loc[i].getRedirection());
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
			return (loc[i].getRedirection());
	}
	return (redir());
}