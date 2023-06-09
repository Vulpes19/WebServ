/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 21:08:22 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "States.hpp"

class ClientInfo
{
    public:
        ClientInfo( void );
        ~ClientInfo( void );
        void	reset( void );
        void    handleReadRequest( void );
        void    handleProcessRequest( void );
        void    handleWriteResponse( void );
        bool    isRequestReceived( void );
        void    changeSet( fd_set &from, fd_set &to );
        enum states &getState( void ) const { return (state); };
        void    setState( enum states &newState ) { state = newState; };
        size_t      getFileSize( const char *path );
        const char  *getFileType( const char *path ) const;
        void    generateResponse( void );
    private:
        char    request[MAX_REQUEST_SIZE + 1];
        char	*path;
        ssize_t bytesReceived;
        ssize_t bytesSent;
        size_t  fileSize;
        SOCKET	socket;
        std::string response;
        std::ifstream file;
        socklen_t	addressLen;
        struct sockaddr_storage address;
        enum states state;
};