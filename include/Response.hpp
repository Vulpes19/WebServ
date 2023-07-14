/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elias <elias@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/12 16:26:06 by elias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "ClientManager.hpp"
#include "Resources.hpp"
#include "CGI.hpp"

class ClientManager;
class CGI;

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
		enum ResponseStates        getResponseFile(Resources &resources);
		enum ResponseStates        getResponseDir( void );
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
        DIR             *dir;
		std::string	    path;
        ErrorResponse   err;
        struct  dirent *entry;
};