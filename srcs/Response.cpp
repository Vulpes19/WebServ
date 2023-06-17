/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:08 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/17 14:08:28 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void    ErrorResponse::errorBadRequest( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 400 Bad Request\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg +=	"Content-Length: 11\r\n\r\nBad Request";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorNotFound( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 404 Not Found\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg +=	"Content-Length: 9\r\n\r\nNot Found";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorForbidden( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 403 Forbidden\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 0\r\n";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

Response::Response( void )
{
	memset(request, 0, sizeof(request));
	bytesReceived = 0;
	bytesSent = 0;
	fileSize = 0;
	socket = -1;
	path = "";
}

Response::~Response( void )
{}

void    Response::setSocket( SOCKET socket )
{
	this->socket = socket;
}

enum ResponseStates    Response::handleReadRequest( Resources &resources )
{
	ssize_t bytesRead = read( socket, request, BSIZE );
	// std::cout << "bytes read: " << bytesRead << std::endl;
	if ( bytesRead > 0 )
	{
		std::cout << "from " << socket << " reading the request... " << bytesReceived << "\n";
		bytesReceived += bytesRead;
		request[bytesRead] = '\0';
		if ( isRequestReceived() )
		{
			std::string toSend(request);
			std::cout << "***********\n";
			std::cout << toSend << std::endl;
			std::cout << "***********\n";
			resources.checkRequest(toSend);
			std::cout << "REQUEST IS RECEIVED\n";
			return (READY_TO_WRITE);
		}
		return (READING);
	}
	// else if ( bytesRead == 0 )
	// {
	// 	std::string toSend(request);
	// 	std::cout << "***********\n";
	// 	std::cout << toSend << std::endl;
	// 	std::cout << "***********\n";
	// 	resources.checkRequest(toSend);
	// 	std::cout << "REQUEST IS RECEIVED\n";
	// 	return (READY_TO_WRITE);
	// }
	else
	{
		std::cerr << "Error reading request\n";
		err.errorBadRequest(socket);
		reset();
		return (RESET);
	}
}

bool	Response::isDirectory( const char *path ) const
{
	struct stat fileInfo;

	if ( stat(path, &fileInfo) == 0 )
		return S_ISDIR(fileInfo.st_mode);
	return (false);
}

enum ResponseStates    Response::getResponseDir( void )
{
	if ( dir == NULL )
	{
		std::ostringstream oss;
		std::string fullPath = "." + path;
		std::cout << "*******" << path << "*******" << "\n";
		if ( access(fullPath.c_str(), F_OK) == -1 )
		{
			std::cout << "NOT FOUND HEREEE\n";
			err.errorNotFound(socket);
			return (RESET);
		}
		if ( access(fullPath.c_str(), R_OK | X_OK) == -1 )
		{
			err.errorForbidden(socket);
			return (RESET);
		}
		dir = opendir(fullPath.c_str());
		if ( dir == NULL )
			std::cerr << "cant open dir\n";
		oss << "HTTP/1.1 200 OK\r\n";
		oss << "Connection: close\r\n";
		oss << "Content-Length: " << fileSize << "\r\n";
		oss << "Content-Type: " << "text/html" << "\r\n";
		oss << "\r\n";
		bytesReceived = send(socket, oss.str().data(), oss.str().size(), 0);
		std::string htmlResponse = "<html><body><ul>";
		bytesReceived += send(socket, htmlResponse.data(), htmlResponse.size(), 0);
		bytesSent += bytesReceived;
		return (READING);
	}
	entry = readdir(dir);
	if ( entry == NULL )
	{
		std::string htmlResponse = "</ul></body></html>";
		bytesReceived = send(socket, htmlResponse.data(), htmlResponse.size(), 0);
		bytesSent += bytesReceived;
		exit(1);
		return (RESET);
	}
	std::string name = entry->d_name;
	if ( name != "." && name != ".." )
	{
		std::string link = "<li><a href=\"" + name + "\">" + name + "</a></li>";
		bytesReceived = send(socket, link.data(), link.size(), 0);
		bytesSent += bytesReceived; 
		return (READING);
	}
	return (RESET);
}

enum ResponseStates    Response::getResponseFile( void )
{
	// std::cout << "generating the response\n";
	if ( !file.is_open() )
	{
		std::string response;
		bytesSent = 0;
		bytesReceived = 0;
		std::ostringstream oss;
		oss << "public" << path;
		std::string fullPath = oss.str();
		if ( access(fullPath.c_str(), F_OK) == -1 )
		{
			err.errorNotFound(socket);
			return (RESET);
		}
		if ( access( fullPath.c_str(), R_OK) == -1 )
		{
			err.errorForbidden(socket);
			return (RESET);
		}
		file.open(fullPath.c_str());
		fileSize = getFileSize(fullPath.c_str());
		std::string ct = getFileType(fullPath.c_str());
		oss.str("");
		oss.clear();
		oss << "HTTP/1.1 200 OK\r\n";
		oss << "Connection: close\r\n";
		oss << "Content-Length: " << fileSize << "\r\n";
		oss << "Content-Type: " << ct << "\r\n";
		oss << "\r\n";
		response = oss.str();
		send( socket, response.data(), response.size(), 0 );
	}
	char buffer[BSIZE];
	file.read(buffer, BSIZE);
	ssize_t bytesRead = file.gcount();
	if ( bytesSent == -1 )
	{
		bytesSent = 0;
		file.close();
		reset();
		std::cout << "READ ERROR\n";
		return (RESET);
	}
	else if ( bytesRead > 0 )
	{
		bytesSent += bytesRead;
		send( socket, buffer, BSIZE, 0 );
		return (READING);
	}
	else
	{
		bytesSent = 0;
		file.close();
		reset();
		std::cout << "FILE IS READ\n";
		return (RESET);
	}
}

enum ResponseStates	Response::postUploadFile( Resources &resources )
{
	std::string filePath(resources.getRequest("URL"));
	std::string toWrite(resources.getRequestBody());

	if ( !toUpload.is_open() )
	{
		toUpload.open(filePath, std::ios::binary);
	}
	toUpload << toWrite;
	toUpload.close();
	std::stringstream oss;
	oss << "HTTP/1.1 201 Created\r\n";
	oss << "Content-Type: " << resources.getRequest("Content-Type") << "\r\n";
	oss << "Content-Length: " << getFileSize(filePath.c_str()) << "\r\n";
	oss << "\r\n";
	oss << toWrite << "\r\n";
	send( socket, oss.str().data(), oss.str().size(), 0);
	reset();
	std::cout << "FILE IS UPLOADED\n";
	return (RESET);
}

bool    Response::handleWriteResponse( Resources &resources )
{
	std::string requestString(request);
	enum ResponseStates ret;

	path = resources.getRequest("URL");
	if ( path.compare("/") == 0 )
		path = "/index.html";
	if ( isDirectory(path.c_str()) )
		ret = getResponseDir();
	if ( resources.getRequest("Method") == "GET")
		ret = getResponseFile();
	if ( resources.getRequest("Method") == "POST" )
	{
		std::cout << "IM IN POST\n";
		ret = postUploadFile(resources);
	}
	if ( ret == READING )
		return (false);
	else
		return (true);
}

bool	Response::isRequestReceived( void ) const
{
	std::string end("\r\n\r\n");

	if ( bytesReceived < (int)end.length() )
		return (false);
	if ( strcmp( request + bytesReceived - end.length(), end.c_str() ) == 0 )
		return (true);
	return (false);
}

ssize_t  Response::getFileSize( const char *path ) const
{
    struct stat fileStat;
    if ( stat(path, &fileStat) == 0 )
        return (fileStat.st_size);
    else
        std::cerr << "cant get file size\n";
    return (-1);
}

std::string  Response::getFileType( const char *path ) const
{
    const char *fileName = strrchr(path, '.');
    static std::map< std::string, std::string > fileTypes;
    if ( fileTypes.empty() )
	{
		fileTypes[".css"] = "text/css";
		fileTypes[".mp4"] = "video/mp4";
		fileTypes[".png"] = "image/png";
		fileTypes[".zip"] = "application/zip";
		fileTypes[".csv"] = "text/csv";
		fileTypes[".gif"] = "image/gif";
		fileTypes[".htm"] = "text/.html";
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
	std::map< std::string, std::string >::iterator it = fileTypes.find(fileName);
    if ( it != fileTypes.end() )
        return ( it->second );
    else
        return "text/plain";
}

void	Response::reset( void )
{
	memset(request, 0, sizeof(request));
	bytesReceived = 0;
	bytesSent = 0;
	fileSize = 0;
	socket = -1;
	path = "";
}