/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/25 11:29:33 by abaioumy         ###   ########.fr       */
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
        Client( void );
        ~Client( void );
        struct ClientInfo   getClient( SOCKET socket ) const;
        void    deleteClient( SOCKET socket );
        const char *getAddress( SOCKET socket );
    private:
        std::list<struct ClientInfo> clients;
};