/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/12 11:28:19 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "States.hpp"
#include "Resources.hpp"

class ClientManager
{
	public:
		ClientManager( void );
		~ClientManager( void );
		void    	reset( void );
		void		unsetSocket( fd_set &readfds, fd_set &writefds );
		void    	handleReadRequest( void );
		bool    	handleProcessRequest( void );
		bool    	handleWriteResponse( void );
		bool    	isRequestReceived( void );
		void    	changeSet( fd_set &from, fd_set &to );
		enum 		states getState( void ) const { return (state); };
		void    	setState( enum states &newState ) { state = newState; };
		bool        generateResponse( void );
		void		createClient( SOCKET listenSocket );
		size_t      getFileSize( const char *path );
		const char  *getFileType( const char *path ) const;
		SOCKET      getSocket( void ) const { return (socket); };
		void        errorBadRequest( void );
		void        errorNotFound( void );
	private:
		char    request[MAX_REQUEST_SIZE + 1];
		std::string	path;
		int		bytesReceived;
		int		bytesSent;
		int  fileSize;
		SOCKET	socket;
		std::string response;
		std::ifstream file;
		socklen_t	addressLen;
		struct sockaddr_storage address;
		enum states state;
		Resources resources;
};