/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:55 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 12:13:20 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"

class ClientInfo
{
    public:
        ClientInfo( void );
        ~ClientInfo( void );
        void	reset( void );
        void    handleReadRequest( void );
        void    handleProcessRequest( void );
        void    handleWriteResponse( void );
        void    changeSet( fd_set &from, fd_set &to );
    private:
        socklen_t	addressLen;
        struct sockaddr_storage address;
        SOCKET	socket;
        char    request[MAX_REQUEST_SIZE + 1];
        int     bytesReceived;
        bool	isReading;
        bool	isWriting;
        char	*path;
};