/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:21:33 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/08 14:17:31 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientManager.hpp"
#include "Libraries.hpp"

class Server
{
    public:
		typedef std::list<ClientManager *>::iterator iterator; 
        Server( void );
        ~Server( void );
        void  createListenSocket( void );
        SOCKET  getListenSocket( void ) const;
        void    setPort( std::string );
        void    setName( std::string );
        void    setLocations( std::vector<Location> );
        std::vector<Location> getLocations( void ) const;
        std::string getName( void ) const { return name;};
    //     ClientManager   *getClient( SOCKET socket );
	// 	void        	deleteClient( ClientManager *cl );
	// 	void			setsManager( fd_set &readfds, fd_set &writefds );
	// 	void    		multiplexing( fd_set &readfds, fd_set &writefds );
    private:
        std::vector<Location>    loc;
		std::list<ClientManager *> clients;
        struct addrinfo *bindAddress;
        std::string port;
        std::string name;
        SOCKET listenSocket;
};