/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/10 17:15:53 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Resources.hpp"

Client::Client( void )
{
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
}

Client::~Client( void )
{}

ClientInfo   *Client::getClient( SOCKET socket, Server srv )
{
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() == socket )
            return (*it);
    }
    ClientInfo *newClient = new ClientInfo();
    newClient->reset();
    newClient->createClient( srv.getListenSocket() );
    clients.push_back(newClient);
    return (newClient);
}

void    Client::deleteClient( ClientInfo *cl )
{
    // close(cl->socket);
    iterator it = clients.begin();
    while ( it != clients.end() )
    {
        if ( (*it)->getSocket() == cl->getSocket() )
            break ;
        ++it;
    }
    if ( it != clients.end() )
        clients.erase(it);
    else
        std::cerr << "dropped client not found\n";
}

void  Client::setsManager( SOCKET socket, fd_set &readfds, fd_set &writefds )
{
    FD_ZERO(&writefds);
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);
    SOCKET maxSocket = socket;
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() > -1 )
        {
            FD_SET((*it)->getSocket(), &readfds);
            if ( (*it)->getState() == WRITE_RESPONSE )
                (*it)->changeSet( readfds, writefds );
            if ( (*it)->getSocket() > maxSocket )
            {
                std::cout << "added the socket\n";
                maxSocket = (*it)->getSocket();
            }
        }
    }
    if ( select( maxSocket + 1, &readfds, &writefds, NULL, &timeout) < 0 )
    {
        std::cerr << "select() failed: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void    Client::errorBadRequest( ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 400 Bad Request\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 11\r\n\r\nBad Request";
    send( cl->getSocket(), errorMsg, strlen(errorMsg), 0 );
    deleteClient( cl );
}

void    Client::errorNotFound( ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 404 Not Found\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 9\r\n\r\nNot Found";
    send( cl->getSocket(), errorMsg, strlen(errorMsg), 0);
}

void    Client::multiplexing( fd_set &readfds, fd_set &writefds )
{
    std::cout << "entering multiplexing\n";
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        std::cout << "SOCKET => " << (*it)->getSocket() << std::endl;
        if ( (*it)->getSocket() == -1 )
            continue ;
        if ( FD_ISSET( (*it)->getSocket(), &readfds) )
        {
            if ( (*it)->getState() == READ_REQUEST )
            {
                (*it)->handleReadRequest();
                continue ;
            }
        }
        if ( FD_ISSET( (*it)->getSocket(), &writefds) )
        {
            if ( (*it)->getState() == WRITE_RESPONSE )
            {
                if ( (*it)->handleWriteResponse() )
                {
                    (*it)->unsetSocket(writefds, readfds);
                    (*it)->reset();
                }
                continue ;
            }
        }
        std::cout << "exiting multiplexing\n";
    }
}