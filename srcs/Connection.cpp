/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/18 15:36:13 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Resources.hpp"

Connection::Connection( void )
{
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    serverName = "NONE";
}

Connection::~Connection( void )
{
    clients.erase(clients.begin(), clients.end());
}

// ClientManager   *Connection::updateClientSettings( iterator it, Server srv )
// {
//     (*it)->reset();
//     (*it)->createClient( srv.getListenSocket() );
//     (*it)->setState( READ_REQUEST);
//     (*it)->setLocations(srv.getLocations());
//     (*it)->setName(srv.getName());
//     (*it)->setHost(srv.getHost());
//     (*it)->setPort(srv.getPort());
//     (*it)->setBodySize(srv.getBodySize());
//     (*it)->setErrorPages(srv.getErrorPages());
//     return (*it); 
// }

ClientManager   *Connection::getClient( SOCKET socket, Server srv, enum SERVER_SETTINGS_STATUS status )
{
    status = UPDATE_CLIENT_SETTINGS;
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() == socket )
        {
            // if ( status == UPDATE_CLIENT_SETTINGS )
            //     return (updateClientSettings( it, srv ) );
            // else
                return (*it);
        }
    }
    ClientManager *newClient = new ClientManager();
    newClient->reset();
    newClient->createClient( srv.getListenSocket() );
    newClient->setState( READ_REQUEST);
    newClient->setLocations(srv.getLocations());
    newClient->setName(srv.getName());
    newClient->setHost(srv.getHost());
    newClient->setPort(srv.getPort());
    newClient->setBodySize(srv.getBodySize());
    newClient->setErrorPages(srv.getErrorPages());
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
        clients.erase(it);
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

// Server	&Connection::getCorrectServer( std::vector<Server> &servers, std::string serverName )
// {
// 	for ( size_t i = 0; i < servers.size(); i++ )
// 	{
// 		if ( servers[i].getName() == serverName )
// 			return (servers[i]);
// 	}
// 	return (servers[0]);
// }

void    Connection::multiplexing( fd_set &readfds, fd_set &writefds, std::string &serverName, std::vector<Server> &servers )
{
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() == -1 )
            continue ;
        if ( FD_ISSET( (*it)->getSocket(), &readfds) )
        {
            if ( (*it)->getState() == READ_REQUEST )
            {
                (*it)->startRead(serverName);
                if ( serverName != "NONE" )
                    (*it)->updateClientSettings(servers, serverName);
                serverName = "NONE";
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