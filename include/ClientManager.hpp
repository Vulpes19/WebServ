/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/18 15:34:00 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "States.hpp"
#include "Resources.hpp"
#include "Response.hpp"
#include "Server.hpp"

class Server;
class Response;
class ClientManager
{
	public:
		ClientManager( void );
		~ClientManager( void );
		void    	reset( void );
		void		unsetSocket( fd_set &readfds, fd_set &writefds );
		void		startRead( std::string & );
		bool		startResponse( void );
		void    	changeSet( fd_set &from, fd_set &to );
		enum 		states getState( void ) const { return (state); };
		void    	setState( enum states newState ) { state = newState; };
		void		createClient( SOCKET listenSocket );
		SOCKET      getSocket( void ) const { return (socket); };
		void		setLocations( std::vector<Location> );
		void		setErrorPages( std::map< std::string, std::string > );
		void		setName( std::string );
		void		setHost( std::string );
		void		setPort( std::string );
		void		setBodySize( ssize_t );
		void		updateClientSettings( std::vector<Server> &srv, std::string serverName );
	private:
		SOCKET		socket;
		Resources	resources;
		Response	*response;
		socklen_t	addressLen;
		std::string	serverName;
		enum states	state;
		struct sockaddr_storage address;
};