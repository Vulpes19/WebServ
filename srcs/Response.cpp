/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:08 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/21 16:10:47 by abaioumy         ###   ########.fr       */
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
	std::cout << errorMsg << std::endl;
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorForbidden( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 403 Forbidden\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 0\r\n";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorInternal( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 500 Internal Server Error\r\n";
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
	if ( bytesRead > 0 )
	{
		bytesReceived += bytesRead;
		request[bytesRead] = '\0';
		std::string toSend(request);
		resources.checkRequest(toSend);
		if ( isRequestReceived(resources) )
		{
			std::cout << "**********\n";
			std::cout << toSend << std::endl;
			std::cout << "**********\n";
			std::cout << "REQUEST IS RECEIVED\n";
			return (READY_TO_WRITE);
		}
		return (READING);
	}
	else if ( bytesRead == 0 )
	{
		std::string toSend(request);
		resources.checkRequest(toSend);
		if ( isRequestReceived(resources) )
		{
			std::cout << "REQUEST IS RECEIVED\n";
			return (READY_TO_WRITE);
		}
		return (READING);
	}
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
		if ( access(fullPath.c_str(), F_OK) == -1 )
		{
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
		fileSize = getFileSize(fullPath.c_str());
		file.open(fullPath.c_str());
		sendResponseHeader( GET, "200 OK", fullPath, NULL );
		// oss.str("");
		// oss.clear();
		// oss << "HTTP/1.1 200 OK\r\n";
		// oss << "Connection: close\r\n";
		// oss << "Content-Length: " << fileSize << "\r\n";
		// oss << "Content-Type: " << ct << "\r\n";
		// oss << "\r\n";
		// response = oss.str();
	}
	char buffer[BSIZE];
	file.read(buffer, BSIZE);
	ssize_t bytesRead = file.gcount();
	buffer[bytesRead] = '\0';
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
		std::cout << "bytes read " << bytesRead << std::endl;
		send( socket, buffer, bytesRead, 0 );
		if ( bytesSent == fileSize )
		{
			std::cout << fileSize << std::endl;
			// exit(1);
			return (RESET);
		}
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
		toUpload.open(filePath.substr(1, filePath.length()).c_str(), std::ios::binary);
		std::cout << "FILE IS OPENED\n";
	}
	toUpload << toWrite;
	toUpload.close();
	sendResponseHeader( POST, "201 Created", filePath.substr(1, filePath.length()), &resources );
	toWrite += "\r\n";
	send( socket, toWrite.data(), toWrite.size(), 0 );
	reset();
	std::cout << "FILE IS UPLOADED\n";
	return (RESET);
}

enum ResponseStates	Response::deleteFile( Resources &resources )
{
	std::string filePath(resources.getRequest("URL"));
	std::ostringstream oss;

	filePath = filePath.substr(1, filePath.length());
	if ( access(filePath.c_str(), F_OK) == -1 )
	{
		err.errorNotFound(socket);
		return (RESET);
	}
	if ( access(filePath.c_str(), W_OK) == -1 )
	{
		err.errorForbidden(socket);
		return (RESET);
	}
	if ( remove(filePath.c_str()) == 0 )
	{
		sendResponseHeader( DELETE, "200 OK", "", &resources );
		send( socket, "File deleted.", 12, 0 );
		
	}
	else
		err.errorInternal(socket);
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
		ret = postUploadFile(resources);
	if ( resources.getRequest("Method") == "DELETE" )
		ret = deleteFile(resources);
	if ( ret == READING )
		return (false);
	else
		return (true);
}

bool	Response::isRequestReceived( Resources &resources ) const
{
	std::string end("\r\n\r\n");
	std::string requestStr(request);
	std::string ret;

	ret = resources.getRequest("Content-Length");
	if ( ret != "NOT FOUND" )
	{
		size_t bodyStart = bytesReceived - std::stoi(ret);
		std::string requestBodyStr = requestStr.substr( bodyStart, bytesReceived );
		if ( (int)requestBodyStr.size() == std::stoi(ret) )
			return (true);
		else
			return (false);
	}
	if ( bytesReceived < (int)end.length() )
		return (false);
	if ( strcmp( request + bytesReceived - end.length(), end.c_str() ) == 0 )
		return (true);
	return (false);
}

void	Response::sendResponseHeader( enum METHODS method, std::string statusCode, std::string fileName, Resources *resources )
{
	std::ostringstream oss;
	oss << "HTTP/1.1 " << statusCode << "\r\n";
	// oss << "Connection: close\r\n";
	//cache-control
	//server name
	//date
	//Connection
	(void)fileName;
	if ( method == GET )
	{
		oss << "Content-Type: " << getFileType(fileName.c_str()) << "\r\n";
		oss << "Content-Length: "<< fileSize << "\r\n";
		oss << "Date: Wed, 21 Jun 2023 14:46:43 GMT\r\n";
		// oss << "Content-Type: text/plain"<< "\r\n";
		// oss << "\r\nwa lhmar tfo";
		// oss << "Content-Length: " << getFileSize(fileName.c_str()) << "\r\n";
	}
	if ( method == POST )
	{
		//oss << location
		oss << "Content-Type: " << resources->getRequest("Content-Type") << "\r\n";
		oss << "Content-Length: " << resources->getRequest("Content-Length") << "\r\n";
	}
	if ( method == DELETE )
	{
		oss << "Content-Type: text/plain\r\n";
		oss << "Content-Length: 12\r\n";
	}
	oss << "\r\n";
	std::cout << oss.str() << std::endl;
	send( socket, oss.str().data(), oss.str().size(), 0 );
	// exit(1);
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