/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:24:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/08 14:23:27 by abaioumy         ###   ########.fr       */
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
    if ( getaddrinfo( name.c_str(), port.c_str(), &hints, &bindAddress) != 0 )
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
        std::string msg(strerror(errno));
        throw excp("bind() failed: " + msg);     
    }
    freeaddrinfo(bindAddress);
    if ( listen(listenSocket, 10) < 0 )
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

void    Server::setLocations( std::vector<Location> loc )
{
    this->loc = loc;
}

std::vector<Location>   Server::getLocations( void ) const
{
    return (loc);
}

// ClientManager   *Server::getClient( SOCKET socket )
// {
//     for ( iterator it = clients.begin(); it != clients.end(); ++it )
//     {
//         if ( (*it)->getSocket() == socket )
//             return (*it);
//     }
//     ClientManager *newClient = new ClientManager();
//     newClient->reset();
//     newClient->createClient( listenSocket );
//     newClient->setState( READ_REQUEST);
//     newClient->setLocations(loc);
//     clients.push_back(newClient);
//     return (newClient);
// }

// void    Server::deleteClient( ClientManager *cl )
// {
//     iterator it = clients.begin();
//     while ( it != clients.end() )
//     {
//         if ( (*it)->getSocket() == cl->getSocket() )
//             break ;
//         ++it;
//     }
//     if ( it != clients.end() )
//         clients.erase(it);
//     else
//         std::cerr << "dropped client not found\n";
// }

// void  Server::setsManager( fd_set &readfds, fd_set &writefds )
// {
// 	struct timeval timeout;

//     SOCKET maxSocket = listenSocket;
//     timeout.tv_sec = 5;
//     timeout.tv_usec = 0;
//     FD_ZERO(&writefds);
//     FD_ZERO(&readfds);
//     FD_SET(listenSocket, &readfds);
//     for ( iterator it = clients.begin(); it != clients.end(); ++it )
//     {
//         if ( (*it)->getSocket() > -1 )
//         {
//             FD_SET((*it)->getSocket(), &readfds);
//             if ( (*it)->getState() == WRITE_RESPONSE )
//                 (*it)->changeSet( readfds, writefds );
//             if ( (*it)->getSocket() > maxSocket )
//                 maxSocket = (*it)->getSocket();
//         }
//     }
//     if ( select( maxSocket + 1, &readfds, &writefds, NULL, &timeout) < 0 )
//     {
//         std::cerr << "select() failed: " << strerror(errno) << std::endl;
//         exit(1);
//     }
// }

// void    Server::multiplexing( fd_set &readfds, fd_set &writefds )
// {
//     std::cout << "entering multiplexing\n";
//     for ( iterator it = clients.begin(); it != clients.end(); ++it )
//     {
//         std::cout << "SOCKET => " << (*it)->getSocket() << std::endl;
//         if ( (*it)->getSocket() == -1 )
//             continue ;
//         if ( FD_ISSET( (*it)->getSocket(), &readfds) && (*it)->getState() == READ_REQUEST )
//         {
//             (*it)->startRead();
//             continue ;
//         }
//         if ( FD_ISSET( (*it)->getSocket(), &writefds) && (*it)->getState() == WRITE_RESPONSE )
//         {
//             if ( (*it)->startResponse() )
//             {
//                 (*it)->unsetSocket(writefds, readfds);
//                 (*it)->reset();
//                 (*it)->setState(READ_REQUEST);
//             }
//             continue ;
//         }
//     }
//     std::cout << "exiting multiplexing\n";
// }