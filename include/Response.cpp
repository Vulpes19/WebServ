/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/13 10:29:08 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"

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
        void        setSocket( SOCKET socket );
        void    	handleReadRequest( void );
		bool    	handleWriteResponse( void );
		bool    	isRequestReceived( void );
		bool        generateResponse( void );
    private:
        char    request[MAX_REQUEST_SIZE + 1];
		std::string	path;
		int		bytesReceived;
		int		bytesSent;
		int  fileSize;
		std::ifstream file;
        SOCKET  socket;
        ErrorResponse   err;
};