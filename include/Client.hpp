/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/25 13:24:24 by abaioumy         ###   ########.fr       */
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
#include <map>
#define SOCKET int
#define MAX_REQUEST_SIZE 2047

struct ClientInfo
{
    socklen_t addressLen;
    struct sockaddr_storage address;
    SOCKET socket;
    char    request[MAX_REQUEST_SIZE + 1];
    int     bytesReceived;
};

class Client
{
    public:
        typedef std::map<SOCKET, struct ClientInfo *>::const_iterator iterator;
        typedef std::pair<SOCKET, struct ClientInfo *> pair; 
        Client( void );
        ~Client( void );
        struct ClientInfo   *getClient( SOCKET socket );
        void    deleteClient( struct ClientInfo *cl );
        const char *getAddress( SOCKET socket );
        fd_set      waitClient( SOCKET socket );
    private:
        std::map<SOCKET, struct ClientInfo *> clients;
};