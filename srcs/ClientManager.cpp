/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:47 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/18 15:40:51 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientManager.hpp"

ClientManager::ClientManager( void )
{
	memset(&address, 0, sizeof(address));
	addressLen = 0;
	socket = -1;
    response = new Response();
    state = READ_REQUEST;
}

ClientManager::~ClientManager( void )
{
    delete response;
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
	addressLen = 0;
	socket = -1;
}

void    ClientManager::startRead( std::string &serverName )
{
    enum ResponseStates ret;
    response->setSocket(socket);
    ret = response->handleReadRequest(resources, serverName);
    if ( ret == READY_TO_WRITE )
        state = WRITE_RESPONSE;
    else if ( ret == RESET )
        reset();
}

bool    ClientManager::startResponse( void )
{
    return (response->handleWriteResponse(resources));
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
    int optval;
    addressLen = sizeof(address);
    socket = accept( listenSocket, (struct sockaddr *) &address, &addressLen);
    int flags = fcntl( socket, F_GETFL, 0  );
    if ( flags == -1 )
    {
        std::cerr << "failed to get flags\n";
        exit(1);
    }
    if ( setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) < 0 )
    {
        std::cerr << "failed to disable SIGPIPE at the socket level\n";
        exit(1);
    }
}

void		ClientManager::updateClientSettings( std::vector<Server> &srv, std::string serverName )
{
    for ( size_t i = 0; i < srv.size(); i++ )
	{
		if ( srv[i].getName() == serverName )
		{
            setName(srv[i].getName());
            setHost(srv[i].getHost());
            setPort(srv[i].getPort());
            setPort(srv[i].getPort());
            setLocations(srv[i].getLocations());
            setBodySize(srv[i].getBodySize());
            setErrorPages(srv[i].getErrorPages());
        }
	}
}

void    ClientManager::setLocations( std::vector<Location> loc )
{
    response->setLocations(loc);
}

void    ClientManager::setErrorPages( std::map< std::string, std::string > errorPages )
{
    response->setErrorPages(errorPages);
}

void    ClientManager::setName( std::string name )
{
    response->setName(name);
}

void    ClientManager::setHost( std::string host )
{
    response->setHost(host);
}

void    ClientManager::setPort( std::string port )
{
    response->setPort(port);
}

void    ClientManager::setBodySize( ssize_t bodySize )
{
    response->setBodySize(bodySize);
}