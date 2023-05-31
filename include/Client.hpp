/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/31 14:17:27 by abaioumy         ###   ########.fr       */
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
#include <map>
#include <sstream>
#include <fstream>
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
        typedef std::map<SOCKET, struct ClientInfo *>::iterator iterator;
        typedef std::pair<SOCKET, struct ClientInfo *> pair; 
        Client( void );
        ~Client( void );
        ClientInfo   *getClient( SOCKET socket, Server srv );
        void    deleteClient( ClientInfo *cl );
        const char      *getAddress( ClientInfo *ci );
        fd_set      waitClient( SOCKET socket );
        void        errorBadRequest( ClientInfo *cl );
        void        errorNotFound( ClientInfo *cl );
        void        serveResource( ClientInfo *cl, std::string path );
        const char  *getFileType( const char *path ) const;
        size_t      getFileSize( const char *path );
        void        checkClients( fd_set reads );
    private:
        std::map<SOCKET, ClientInfo *> clients;
        fd_set reads;
};