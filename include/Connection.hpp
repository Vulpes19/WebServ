/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/05 17:06:45 by abaioumy         ###   ########.fr       */
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
		void			setsManager( SOCKET socket, fd_set &readfds, fd_set &writefds );
		void    		multiplexing( fd_set &readfds, fd_set &writefds );
	private:
		std::list<ClientManager *> clients;
		struct timeval timeout;
};