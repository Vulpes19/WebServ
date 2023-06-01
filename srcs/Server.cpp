/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:24:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/01 12:31:04 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void )
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ( getaddrinfo( 0, "8080", &hints, &bindAddress) != 0 )
    {
        std::cerr << "getaddrinfo() failed: " << strerror(errno) << std::endl;
    }
}

Server::~Server( void )
{}

void  Server::createListenSocket( void )
{
    listenSocket = socket( bindAddress->ai_family, bindAddress->ai_socktype, bindAddress->ai_protocol );
    if ( listenSocket == -1 )
    {
        std::cerr << "socket() failed: " << strerror(errno) << std::endl;
    }
    const int enable = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    if ( bind( listenSocket, bindAddress->ai_addr, bindAddress->ai_addrlen) != 0 )
    {
        std::cerr << "bind() failed: " << strerror(errno) << std::endl;     
    }
    freeaddrinfo(bindAddress);
    if ( listen(listenSocket, 10) < 0 )
    {
        std::cerr << "listen() failed: " << strerror(errno) << std::endl;
    }
}

SOCKET  Server::getListenSocket( void ) const
{
    return (listenSocket);
}