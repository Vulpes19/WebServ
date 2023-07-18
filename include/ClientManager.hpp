/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/16 12:15:28 by abaioumy         ###   ########.fr       */
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
		void    	changeSet( fd_set &from, fd_set &to );
		enum 		states getState( void ) const { return (state); };
		void    	setState( enum states newState ) { state = newState; };
		void		createClient( SOCKET listenSocket );
		SOCKET      getSocket( void ) const { return (socket); };
		void		setLocations( std::vector<Location> );
		void		setErrorPages( std::map< std::string, std::string > );
		void		setName( std::string );
		void		setHost( std::string );
		void		setBodySize( ssize_t );
	private:
		SOCKET		socket;
		Resources	resources;
		Response	*response;
		socklen_t	addressLen;
		enum states	state;
		struct sockaddr_storage address;
};