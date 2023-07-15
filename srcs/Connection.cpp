/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/15 14:54:33 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Resources.hpp"

Connection::Connection( void )
{
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
}

Connection::~Connection( void )
{
    clients.erase(clients.begin(), clients.end());
}

ClientManager   *Connection::getClient( SOCKET socket, Server srv )
{
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() == socket )
            return (*it);
    }
    ClientManager *newClient = new ClientManager();
    newClient->reset();
    newClient->createClient( srv.getListenSocket() );
    newClient->setState( READ_REQUEST);
    newClient->setLocations(srv.getLocations());
    newClient->setName(srv.getName());
    newClient->setHost(srv.getHost());
    newClient->setBodySize(srv.getBodySize());
    clients.push_back(newClient);
    return (newClient);
}

void    Connection::deleteClient( ClientManager *cl )
{
    iterator it = clients.begin();
    while ( it != clients.end() )
    {
        if ( (*it)->getSocket() == cl->getSocket() )
            break ;
        ++it;
    }
    if ( it != clients.end() )
    {
        std::cout << "client is deleted :D\n";
        clients.erase(it);
    }
    else
        std::cerr << "dropped client not found\n";
}

void  Connection::setsManager( std::vector<Server> servers, fd_set &readfds, fd_set &writefds )
{
    FD_ZERO(&writefds);
    FD_ZERO(&readfds);
    SOCKET maxSocket;
    maxSocket = servers[0].getListenSocket();
    // std::map<std::string, Server>::iterator it = servers.begin();
    for ( size_t i = 0; i < servers.size(); i++ )
    {
        SOCKET serverSocket = servers[i].getListenSocket();
        if ( serverSocket > maxSocket )
            maxSocket = serverSocket;
        FD_SET(serverSocket, &readfds);
    }
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() > -1 )
        {
            FD_SET((*it)->getSocket(), &readfds);
            if ( (*it)->getState() == WRITE_RESPONSE )
                (*it)->changeSet( readfds, writefds );
            if ( (*it)->getSocket() > maxSocket )
                maxSocket = (*it)->getSocket();
        }
    }
    if ( select( maxSocket + 1, &readfds, &writefds, NULL, &timeout) < 0 )
        throw excp("select() failed");
}

void    Connection::multiplexing( fd_set &readfds, fd_set &writefds )
{
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() == -1 )
            continue ;
        if ( FD_ISSET( (*it)->getSocket(), &readfds) )
        {
            if ( (*it)->getState() == READ_REQUEST )
            {
                (*it)->startRead();
                continue ;
            }
        }
        if ( FD_ISSET( (*it)->getSocket(), &writefds) )
        {
            if ( (*it)->getState() == WRITE_RESPONSE )
            {
                if ( (*it)->startResponse() )
                {
                    (*it)->unsetSocket(writefds, readfds);
                    (*it)->reset();
                    (*it)->setState(READ_REQUEST);
                }
                continue ;
            }
        }
    }
}