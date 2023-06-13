/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:08 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/13 11:16:27 by abaioumy         ###   ########.fr       */
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
{}

Response::~Response( void )
{}

void    Response::setSocket( SOCKET socket )
{
    this->socket = socket;
}

void    Response::handReadRequest( void )
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
        // reset();
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
        if ( access(fullPath.c_str(), F_OK) == -1 )
        {
            errorNotFound();
            reset();
            state = READ_REQUEST;
            return (false);
        }
        if ( access( fullPath.c_str(), R_OK) == -1 )
        {
            errorForbidden();
            reset();
            state = READ_REQUEST;
            return (false);
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
        send( socket, buffer, BSIZE, 0 );
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
        path = "/index.html";
    if ( generateResponse() )
        return (true);
    else
        return (false);
}
