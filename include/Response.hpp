/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/17 11:50:25 by abaioumy         ###   ########.fr       */
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
		enum ResponseStates	handleReadRequest( Resources & );
		enum ResponseStates	getResponseFile( void );
		enum ResponseStates	getResponseDir( void );
		enum ResponseStates	postUploadFile( Resources &resources );
		bool		handleWriteResponse( Resources & );
		bool		isRequestReceived( void ) const;
		ssize_t     getFileSize( const char * ) const;
		std::string	getFileType( const char * ) const;
		bool		isDirectory( const char *path ) const;
		void		reset( void );
	private:
		char    request[MAX_REQUEST_SIZE + 1];
		int		bytesReceived;
		int		bytesSent;
		int     fileSize;
		SOCKET  socket;
		DIR     *dir;
		ErrorResponse   err;
		std::ifstream   file;
		std::ofstream	toUpload;
		std::string	    path;
		struct  dirent *entry;
};