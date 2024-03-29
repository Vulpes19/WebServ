/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:47 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/27 16:46:23 by abaioumy         ###   ########.fr       */
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
    // resources.clear();
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
        throw excp("failed to get flags");
    else
    {
        flags |= O_NONBLOCK; // Set the O_NONBLOCK flag
        int result = fcntl(socket, F_SETFL, flags);
        if (result == -1)
            throw excp("Failed to set socket to non-blocking");
    }
    if ( setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval)) < 0 )
        throw excp("failed to disable SIGPIPE at the socket level");
}

void		ClientManager::updateClientSettings( std::vector<Server> &srv, std::string serverName )
{
    for ( size_t i = 0; i < srv.size(); i++ )
	{
		if ( srv[i].getName() == serverName )
		{
            if ( srv[i].getPort() != response->getPort() )
                return ;
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

void    ClientManager::setUpload( std::string upload )
{
    response->setUpload(upload);
}