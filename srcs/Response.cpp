/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:08 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 13:24:47 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void    ErrorResponse::errorBadRequest( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 400 Bad Request\r\n"
							"Connection: close\r\n"
							"Content-Length: 11\r\n\r\nBad Request";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorNotFound( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 404 Not Found\r\n"
							"Connection: close\r\n"
							"Content-Length: 9\r\n\r\nNot Found";
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
	// std::cout << "socket in read request func: " << socket << std::endl;
	ssize_t bytesRead = read( socket, request, 1000 );
	std::cout << "bytes read: " << bytesRead << std::endl;
	if ( bytesRead > 0 )
	{
		std::cout << "reading the request... " << bytesReceived << "\n";
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
			// cl.setState(WRITE_RESPONSE);
			return (READY_TO_WRITE);
		}
		return (READING);
	}
	// else if ( bytesRead == 0 )
	// {
	// 	std::cout << "reset\n";
	// 	reset();
	// 	// cl.reset();
	// 	return ()
	// }
	else
	{
		std::cerr << "Error reading request\n";
		err.errorBadRequest(socket);
		reset();
		// cl.reset();
		return (RESET);
	}
}


enum ResponseStates    Response::generateResponse( void )
{
	// std::cout << "generating the response\n";
	if ( !file.is_open() )
	{
		std::string response;
		bytesSent = 0;
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
		// if ( !file.is_open() )
		// {
		//     std::cerr << "Error: file not found\n";
		//     errorNotFound();
		//     reset();
		//     state = READ_REQUEST;
		//     return (false);
		// }
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
		// cl.setState(READ_REQUEST);
		file.close();
		reset();
		// cl.reset();
		std::cout << "READ ERROR\n";
		return (RESET);
	}
	else if ( bytesRead > 0 )
	{
		// std::cout << "reading the file...\n";
		bytesSent += bytesRead;
		send( socket, buffer, BSIZE, 0 );
		return (READING);
	}
	else
	{
		bytesSent = 0;
		// cl.setState(READ_REQUEST);
		file.close();
		std::cout << "FILE IS READ\n";
		return (RESET);
	}
	// if ( bytesSent == (ssize_t)fileSize )
	// {
	//     bytesSent = 0;
	//     state = READ_REQUEST;
	//     file.close();
	//     std::cout << "FILE IS READ\n";
	//     return (true);
	// }
	// else if ( bytesRead > 0 )
	// {
	//     std::cout << "reading the file...\n";
	//     bytesSent += bytesRead;
	//     send( socket, buffer, BSIZE, 0 );
	//     return (false);
	// }
	// else
	// {
	//     bytesSent = 0;
	//     state = READ_REQUEST;
	//     file.close();
	//     std::cout << "FILE IS READ\n";
	//     return (true);
	// }
}

bool    Response::handleWriteResponse( Resources &resources )
{
	std::string requestString(request);
	enum ResponseStates ret;

	path = resources.getRequest("URL");
	if ( strcmp(path.c_str(), "/") == 0 )
		path = "/index.html";
	ret = generateResponse();
	if ( ret == READING )
		return (false);
	else
		return (true);
}

bool	Response::isRequestReceived( void )
{
	std::string end("\r\n\r\n");

	if ( bytesReceived < (int)end.length() )
		return (false);
	if ( strcmp( request + bytesReceived - end.length(), end.c_str()) == 0 )
		return (true);
	return (false);
}

size_t  Response::getFileSize( const char *path ) const
{
    struct stat fileStat;
    if ( stat(path, &fileStat) == 0 )
    {
        std::cout << "file size is " << fileStat.st_size << std::endl;
        return (fileStat.st_size);
    }
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