/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:21:33 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/15 08:33:02 by abaioumy         ###   ########.fr       */
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
        void    setHost( std::string );
        void    setBodySize( size_t );
        void    setLocations( std::vector<Location> );
        std::vector<Location> getLocations( void ) const;
        std::string getName( void ) const { return name;};
        std::string getHost( void ) const { return host;};
        size_t getBodySize( void ) const { return bodySize;};
    private:
        std::vector<Location>    loc;
		std::list<ClientManager *> clients;
        struct addrinfo *bindAddress;
        std::string port;
        std::string name;
        std::string host;
        size_t      bodySize;
        SOCKET listenSocket;
};