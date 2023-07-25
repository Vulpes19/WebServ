/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:24:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/25 18:23:05 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void )
{}

Server::~Server( void )
{}

void  Server::createListenSocket( void )
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // std::cout << host << std::endl;
    if ( getaddrinfo( host.c_str(), port.c_str(), &hints, &bindAddress) != 0 )
    {
        std::string msg(strerror(errno));
        throw excp("getaddrinfo() failed: " + msg);
    }
    listenSocket = socket( bindAddress->ai_family, bindAddress->ai_socktype, bindAddress->ai_protocol );
    if ( listenSocket == -1 )
    {
        std::string msg(strerror(errno));
        throw excp("socket() failed: " + msg);
    }
    const int enable = 1;
    if ( setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0 )
    {
        std::string msg(strerror(errno));
        throw excp("setsockopt(SO_REUSEADDR) failed: " + msg);    
    }
    if ( bind( listenSocket, bindAddress->ai_addr, bindAddress->ai_addrlen) != 0 )
    {
        if ( errno != EADDRINUSE )
        {            
            std::string msg(strerror(errno));
            throw excp("bind() failed: " + msg);
        }     
    }
    freeaddrinfo(bindAddress);
    if ( listen(listenSocket, 200) < 0 )
    {
        std::string msg(strerror(errno));
        throw excp("listen() failed: " + msg);     
    }
}

SOCKET  Server::getListenSocket( void ) const
{
    return (listenSocket);
}

void    Server::setPort( std::string port )
{
    this->port = port;
}

void    Server::setName( std::string name )
{
    this->name = name;
}

void    Server::setHost( std::string host )
{
    this->host = host;
}

void    Server::setLocations( std::vector<Location> loc )
{
    this->loc = loc;
}

void    Server::setBodySize( ssize_t bodySize )
{
    this->bodySize = bodySize;
}

void    Server::setUpload( std::string upload )
{
    this->upload = upload;
}

void    Server::setErrorPages( std::map< std::string, std::string > errorPages )
{
    this->errorPages = errorPages;
}

std::vector<Location>   Server::getLocations( void ) const
{
    return (loc);
}
