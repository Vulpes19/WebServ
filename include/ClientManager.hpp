/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 11:12:23 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "States.hpp"
#include "Resources.hpp"
#include "Response.hpp"

class Response;
class ClientManager
{
	public:
		ClientManager( void );
		~ClientManager( void );
		void    	reset( void );
		void		unsetSocket( fd_set &readfds, fd_set &writefds );
		void		startRead( void );
		bool		startResponse( void );
		// void    	handleReadRequest( void );
		// bool    	handleProcessRequest( void );
		// bool    	handleWriteResponse( void );
		// bool    	isRequestReceived( void );
		void    	changeSet( fd_set &from, fd_set &to );
		enum 		states getState( void ) const { return (state); };
		void    	setState( enum states newState ) { state = newState; };
		// bool        generateResponse( void );
		void		createClient( SOCKET listenSocket );
		// size_t      getFileSize( const char *path );
		// std::string	getFileType( const char *path ) const;
		SOCKET      getSocket( void ) const { return (socket); };
		// Response	&getResponse( void ) { return (response); };
		// void        errorBadRequest( void );
		// void        errorNotFound( void );
		// void    	errorForbidden( void );
	private:
		// char    request[MAX_REQUEST_SIZE + 1];
		// std::string	path;
		// int		bytesReceived;
		// int		bytesSent;
		// int  fileSize;
		SOCKET	socket;
		// std::string response;
		// std::ifstream file;
		socklen_t	addressLen;
		struct sockaddr_storage address;
		enum states state;
		Resources resources;
		Response	*response;
};