/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:21:33 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/26 19:57:05 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientManager.hpp"
#include "Libraries.hpp"

class ClientManager;
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
        void    setBodySize( ssize_t );
        void    setUpload( std::string );
        void    setLocations( std::vector< Location > );
        void    setErrorPages( std::map< std::string, std::string > );
        std::vector<Location> getLocations( void ) const;
        std::string getName( void ) const { return name;};
        std::string getHost( void ) const { return host;};
        std::string getPort( void ) const { return port;};
        std::string getUpload( void ) const { return upload;};
        // std::string getSize( void ) const { return size;};
        ssize_t getBodySize( void ) const { return bodySize;};
        std::map< std::string, std::string > getErrorPages( void ) const { return errorPages; };
    private:
        std::vector< Location >    loc;
		std::list< ClientManager * > clients;
        std::map< std::string, std::string > errorPages;
        std::string port;
        std::string name;
        std::string host;
        std::string upload;
        ssize_t      bodySize;
        SOCKET listenSocket;
};