/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:44:56 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/26 14:42:53 by codespace        ###   ########.fr       */
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
        void        errorBadRequest( struct ClientInfo *cl );
        void        errorNotFound( struct ClientInfo *cl );
        void        serveResource( struct ClientInfo *cl, char *path );
        const char  *getFileType( const char *path ) const;
        size_t      getFileSize( const char *path );
    private:
        std::map<SOCKET, struct ClientInfo *> clients;
        fd_set reads;
};