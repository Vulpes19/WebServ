/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 11:40:34 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "ClientManager.hpp"
#include "Resources.hpp"

class ClientManager;

struct ErrorResponse
{
    void        errorBadRequest( SOCKET socket );
    void        errorNotFound( SOCKET socket );
    void    	errorForbidden( SOCKET socket );
};

class Response
{
    public:
        Response( void );
        ~Response( void );
        void        setSocket( SOCKET );
        enum ResponseStates    	handleReadRequest( Resources & );
		bool    	handleWriteResponse( Resources & );
		bool    	isRequestReceived( void );
		enum ResponseStates        generateResponse( void );
		size_t      getFileSize( const char * ) const;
		std::string	getFileType( const char * ) const;
        void        reset( void );
    private:
        char    request[MAX_REQUEST_SIZE + 1];
		int		bytesReceived;
		int		bytesSent;
		int     fileSize;
        SOCKET  socket;
		std::ifstream   file;
		std::string	    path;
        ErrorResponse   err;
};