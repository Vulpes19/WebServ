/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 12:06:03 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "Server.hpp"

class Client
{
	public:
		typedef std::list<struct ClientInfo *>::iterator iterator; 
		Client( void );
		~Client( void );
		ClientInfo   *getClient( SOCKET socket, Server srv );
		void        deleteClient( ClientInfo *cl );
		const char      *getAddress( ClientInfo *ci );
		void		setsManager( SOCKET socket, fd_set &readfds, fd_set &writefds )
		void        errorBadRequest( ClientInfo *cl );
		void        errorNotFound( ClientInfo *cl );
		void        serveResource( ClientInfo *cl, std::string path );
		const char  *getFileType( const char *path ) const;
		size_t      getFileSize( const char *path );
		void        checkClients( fd_set &readfds );
		void        readyToWrite( SOCKET socket, fd_set &readfds );
		void    	readyToRead( SOCKET socket, fd_set &readfds );
		// void        setReadfds( fd_set &readfds ) { this->readfds = readfds; };
	private:
		std::list<ClientInfo *> clients;
		fd_set  writefds;
		// fd_set  readfds;
		struct timeval timeout;
};