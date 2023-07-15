/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:47 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/15 08:42:41 by abaioumy         ###   ########.fr       */
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

void    ClientManager::startRead( void )
{
    enum ResponseStates ret;
    response->setSocket(socket);
    ret = response->handleReadRequest(resources);
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

void    ClientManager::setLocations( std::vector<Location> loc )
{
    response->setLocations(loc);
}

void    ClientManager::setName( std::string name )
{
    response->setName(name);
}

void    ClientManager::setHost( std::string host )
{
    response->setHost(host);
}

void    ClientManager::setBodySize( ssize_t bodySize )
{
    response->setBodySize(bodySize);
}