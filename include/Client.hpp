/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/06 10:59:23 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <list>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include "Server.hpp"
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
        typedef std::list<struct ClientInfo *>::iterator iterator; 
        Client( void );
        ~Client( void );
        ClientInfo   *getClient( SOCKET socket, Server srv );
        void        deleteClient( ClientInfo *cl );
        const char      *getAddress( ClientInfo *ci );
        fd_set      waitClient( SOCKET socket );
        void        errorBadRequest( ClientInfo *cl );
        void        errorNotFound( ClientInfo *cl );
        void        serveResource( ClientInfo *cl, std::string path );
        const char  *getFileType( const char *path ) const;
        size_t      getFileSize( const char *path );
        void        checkClients( fd_set &reads );
    private:
        std::list<ClientInfo *> clients;
        fd_set reads;
};