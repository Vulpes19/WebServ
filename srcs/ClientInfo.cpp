/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:47 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 21:09:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientInfo.hpp"

ClientInfo::ClientInfo( void )
{
	memset(&address, 0, sizeof(address));
	memset(request, 0, sizeof(request));
	addressLen = 0;
	socket = -1;
	bytesReceived = 0;
	path = NULL;
}

ClientInfo::~ClientInfo( void )
{
	close(socket);
}

void    ClientInfo::reset( void )
{
	memset(&address, 0, sizeof(address));
	memset(request, 0, sizeof(request));
	addressLen = 0;
	socket = -1;
	bytesReceived = 0;
	path = NULL;
}

bool	ClientInfo::isRequestReceived( void )
{
	std::string end("\r\n\r\n");

	if ( bytesReceived < end.length() )
		return (false);
	if ( strcmp( request + bytesReceived - end.length(), end.c_str()) == 0 )
		return (true);
	return (false);
}

const char  *ClientInfo::getFileType( const char *path ) const
{
    const char *fileName = strrchr(path, '.');
    if ( strcmp(fileName, ".css") == 0 )
        return ("text/css");
    if ( strcmp(fileName, ".mp4") == 0 )
        return ("video/mp4");
    if ( strcmp(fileName, ".csv") == 0 )
        return ("text/csv");
    if ( strcmp(fileName, ".gif") == 0 )
        return ("image/gif");
    if ( strcmp(fileName, ".htm") == 0 || strcmp(fileName, ".html") == 0 )
        return ("text/html");
    if ( strcmp(fileName, ".ico") == 0 )
        return ("image/x-icon");
    if ( strcmp(fileName, ".jpeg") == 0 || strcmp(fileName, ".jpg") == 0 )
        return ("image/jpeg");
    if ( strcmp(fileName, ".js") == 0 )
        return ("application/javascript");
    if ( strcmp(fileName, ".json") == 0 )
        return ("application/json");
    if ( strcmp(fileName, ".png") == 0 )
        return ("image/png");
    if ( strcmp(fileName, ".pdf") == 0 )
        return ("application/pdf");
    if ( strcmp(fileName, ".svg") == 0 )
        return ("image/svg+xml");
    if ( strcmp(fileName, ".txt") == 0 )
        return ("text/plain");
    return "text/plain";
}

bool    ClientInfo::handleReadRequest( void )
{
	ssize_t bytesRead = read( socket, request, 1000 );
	if ( bytesRead > 0 )
	{
		bytesReceived += bytesRead;
		request[bytesRead] = '\0';
		if ( isRequestReceived() )
			state = PROCESS_REQUEST;
	}
	else if ( bytesRead == 0 )
		reset();
	else
		this->~ClientInfo();
}

void    ClientInfo::generateResponse( void )
{
    if ( !file.is_open() )
    {
        std::ostringstream oss;
        oss << "public" << path;
        std::string fullPath = oss.str();
        std::cout << fullPath << std::endl;
        file.open(fullPath.c_str());
        if ( !file.is_open() )
        {
            std::cerr << "error\n";
            exit(1);
        }
        int fileSize = getFileSize(fullPath.c_str());
        if ( fileSize == -1 )
            return ;
        const char *ct = getFileType(fullPath.c_str());
        oss.str("");
        oss.clear();
        oss << "HTTP/1.1 200 OK\r\n";
        oss << "Connection: close\r\n";
        oss << "Content-Length: " << fileSize << "\r\n";
        oss << "Content-Type: " << ct << "\r\n";
        oss << "\r\n";
        response += oss.str();
    }
    char buffer[BSIZE];
    file.read(buffer, BSIZE);
    ssize_t bytesRead = file.gcount();
    std::string tmp(buffer);
    if ( bytesRead > 0 )
    {
        bytesSent += bytesRead;
        response += tmp;
    }
    else
    {
        bytesSent = 0;
        state = WRITE_RESPONSE;
        file.close();
    }
}

void    ClientInfo::handleProcessRequest( void )
{
    std::string requestString(request);

    std::cout << "received request: \n" << requestString << std::endl;
    path = "/jake.mp4";
    if ( strcmp(path, "/") == 0 )
    {
        std::cout << "path is /" << std::endl;
        path = "/jake.mp4";
    }
    // if ( path.length() > 100 )
    // {
    //     errorBadRequest(cl);
    //     return ;
    // }
    // if ( strstr(path.c_str(), "..") )
    // {
    //     errorNotFound(cl);
    //     return ;
    // }
    if ( !file.is_open() )
        generateResponse();
}

void    ClientInfo::handleWriteResponse( void )
{
    if ( bytesSent < fileSize )
    {
        send( socket, response.data(), BSIZE, NULL );
        bytesSent += BSIZE;
    }
    else
    {
        this->~ClientInfo();
    }
}

void    ClientInfo::changeSet( fd_set &from, fd_set &to )
{
    FD_CLR( socket, &from );
    FD_SET( socket, &to );
}