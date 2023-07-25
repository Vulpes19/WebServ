/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vulpes <vulpes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/20 12:40:23 by vulpes           ###   ########.fr       */
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
		void        	deleteClient( ClientManager *cl );
		void			setsManager( std::vector<Server> &servers, fd_set &readfds, fd_set &writefds );
		void    		multiplexing( fd_set &readfds, fd_set &writefds, std::string &serverName, std::vector<Server> &servers );
		// ClientManager	*updateClientSettings( iterator it, Server srv );
		// Server			&getCorrectServer( std::vector<Server> &servers, std::string serverName )
	private:
		std::list<ClientManager *> clients;
		std::string					serverName;
		struct timeval timeout;
};