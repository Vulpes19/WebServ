/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/10 16:51:16 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "States.hpp"

class ClientInfo
{
	public:
		ClientInfo( void );
		~ClientInfo( void );
		void    reset( void );
		void	unsetSocket( fd_set &readfds, fd_set &writefds );
		void    handleReadRequest( void );
		bool    handleProcessRequest( void );
		bool    handleWriteResponse( void );
		bool    isRequestReceived( void );
		void    changeSet( fd_set &from, fd_set &to );
		enum states getState( void ) const { return (state); };
		void    setState( enum states &newState ) { state = newState; };
		size_t      getFileSize( const char *path );
		const char  *getFileType( const char *path ) const;
		bool        generateResponse( void );
		SOCKET      getSocket( void ) const { return (socket); };
		void		createClient( SOCKET listenSocket );
		struct sockaddr_storage getAddress( void ) const { return (address); };
		socklen_t      getAddressLen( void ) const { return (addressLen); };
		void		setAddressLen( socklen_t newLen ) { addressLen = newLen; };
		void		setSocket( SOCKET newSocket ) { socket = newSocket; };
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
};