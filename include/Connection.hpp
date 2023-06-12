/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/12 11:18:55 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "Server.hpp"
#include "ClientManager.hpp"

class Connection
{
	public:
		typedef std::list<ClientManager *>::iterator iterator; 
		Connection( void );
		~Connection( void );
		ClientManager   *getClient( SOCKET socket, Server srv );
		void        deleteClient( ClientManager *cl );
		// const char      *getAddress( ClientManager *ci );
		void		setsManager( SOCKET socket, fd_set &readfds, fd_set &writefds );
		// void        errorBadRequest( ClientManager *cl );
		// void        errorNotFound( ClientManager *cl );
		void		checkClients( fd_set &readfds, fd_set &writefds );
		// void        serveResource( ClientManager *cl, std::string path );
		// const char  *getFileType( const char *path ) const;
		void    	multiplexing( fd_set &readfds, fd_set &writefds );
		// void        readyToWrite( SOCKET socket, fd_set &readfds );
		// void    	readyToRead( SOCKET socket, fd_set &readfds );
		// void        setReadfds( fd_set &readfds ) { this->readfds = readfds; };
	private:
		std::list<ClientManager *> clients;
		struct timeval timeout;
};