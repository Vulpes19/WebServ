/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:21:33 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/27 13:03:14 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#define SOCKET int

class Server
{
    public:
        Server( void );
        ~Server( void );
        void  createListenSocket( void );
        SOCKET  getListenSocket( void ) const;
    private:
        struct addrinfo *bindAddress;
        SOCKET listenSocket;
};