/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:47 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/12 15:16:10 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"

ClientManager::ClientManager( void )
{
	memset(&address, 0, sizeof(address));
	memset(request, 0, sizeof(request));
	addressLen = 0;
	socket = -1;

	bytesReceived = 0;
	path = "";
}

ClientManager::~ClientManager( void )
{
	close(socket);
}

void    ClientManager::unsetSocket( fd_set &readfds, fd_set &writefds )
{
    FD_CLR(socket, &readfds);
    FD_CLR(socket, &writefds);
}

void    ClientManager::reset( void )
{
    close(socket);
	memset(&address, 0, sizeof(address));
	memset(request, 0, sizeof(request));
	addressLen = 0;
	socket = -1;
	bytesReceived = 0;
	path = "";
}

bool	ClientManager::isRequestReceived( void )
{
	std::string end("\r\n\r\n");

	if ( bytesReceived < (int)end.length() )
		return (false);
	if ( strcmp( request + bytesReceived - end.length(), end.c_str()) == 0 )
		return (true);
	return (false);
}

size_t  ClientManager::getFileSize( const char *path )
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

std::string  ClientManager::getFileType( const char *path ) const
{
    const char *fileName = strrchr(path, '.');
    static std::map< std::string, std::string > fileTypes;
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
    std::map< std::string, std::string >::iterator it = fileTypes.find(fileName);
    if ( it != fileTypes.end() )
        return ( it->second );
    else
        return "text/plain";
}

void    ClientManager::handleReadRequest( void )
{
	ssize_t bytesRead = read( socket, request, 1000 );
	if ( bytesRead > 0 )
	{
        std::cout << "reading the request...\n";
		bytesReceived += bytesRead;
		request[bytesRead] = '\0';
		if ( isRequestReceived() )
        {
            std::string toSend(request);
            std::cout << toSend << std::endl;
            resources.checkRequest(toSend);
            std::cout << "REQUEST IS RECEIVED\n";
			state = WRITE_RESPONSE;
        }
	}
	else if ( bytesRead == 0 )
    {
        std::cout << "reset\n";
		reset();
    }
    else
    {
        std::cerr << "Error reading request\n";
        errorBadRequest();
        reset();
    }
}

bool    ClientManager::generateResponse( void )
{
    std::cout << "generating the response\n";
    if ( !file.is_open() )
    {
        bytesSent = 0;
        std::ostringstream oss;
        oss << "public" << path;
        std::string fullPath = oss.str();
        std::cout << fullPath << std::endl;
        file.open(fullPath.c_str());
        if ( !file.is_open() )
        {
            std::cerr << "Error: file not found\n";
            errorNotFound();
            reset();
            state = READ_REQUEST;
            return (false);
        }
        fileSize = getFileSize(fullPath.c_str());
        std::cout << " file size: " << fileSize << std::endl;
        std::string ct = getFileType(fullPath.c_str());
        oss.str("");
        oss.clear();
        oss << "HTTP/1.1 200 OK\r\n";
        oss << "Connection: close\r\n";
        oss << "Content-Length: " << fileSize << "\r\n";
        oss << "Content-Type: " << ct << "\r\n";
        oss << "\r\n";
        response += oss.str();
        send( socket, response.data(), response.size(), 0 );
    }
    char buffer[BSIZE];
    file.read(buffer, BSIZE);
    ssize_t bytesRead = file.gcount();
    std::cout << " bytes read: " << bytesRead << std::endl;
    if ( bytesSent == (ssize_t)fileSize )
    {
        bytesSent = 0;
        state = READ_REQUEST;
        file.close();
        std::cout << "FILE IS READ\n";
        return (true);
    }
    else if ( bytesRead > 0 )
    {
        std::cout << "reading the file...\n";
        bytesSent += bytesRead;
        // std::cout << "read: " << bytesSent << " " << " file size: " << fileSize << std::endl;
        send( socket, buffer, BSIZE, 0 );
        // bytesSent += BSIZE;
        return (false);
    }
    else
    {
        bytesSent = 0;
        state = READ_REQUEST;
        file.close();
        std::cout << "FILE IS READ\n";
        return (true);
    }
}

bool    ClientManager::handleWriteResponse( void )
{
    std::string requestString(request);

    path = resources.getRequest("URL");
    if ( strcmp(path.c_str(), "/") == 0 )
    {
        // std::cout << "path is /" << std::endl;
        path = "/index.html";
    }
    if ( generateResponse() )
        return (true);
    else
        return (false);
}

void    ClientManager::changeSet( fd_set &from, fd_set &to )
{
    if ( socket > 0 )
    {
        FD_CLR( socket, &from );
        FD_SET( socket, &to );
    }
}

void    ClientManager::createClient( SOCKET listenSocket )
{
    addressLen = sizeof(address);
    socket = accept( listenSocket, (struct sockaddr *) &address, &addressLen);
    int flags = fcntl( socket, F_GETFL, 0  );
    if ( flags == -1 )
        std::cerr << "failed to get flags\n";
}

void    ClientManager::errorBadRequest( void )
{
    std::string errorMsg = "HTTP/1.1 400 Bad Request\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 11\r\n\r\nBad Request";
    send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ClientManager::errorNotFound( void )
{
    std::string errorMsg = "HTTP/1.1 404 Not Found\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 9\r\n\r\nNot Found";
    send( socket, errorMsg.data(), errorMsg.size(), 0);
}