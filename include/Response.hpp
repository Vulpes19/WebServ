/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/07 13:16:35 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "ClientManager.hpp"
#include "Resources.hpp"

class ClientManager;

struct ErrorResponse
{
	void        errorBadRequest( SOCKET );
	void        errorNotFound( SOCKET );
	void    	errorForbidden( SOCKET );
	void    	errorInternal( SOCKET );
};

struct ResponseHelper
{
	ssize_t     getFileSize( const char * ) const;
	bool		isDirectory( std::string ) const;
	const std::string	getFileType( const char * ) const;
	const std::string	getCurrentTime( void ) const;
	const std::string	getFileLocation( const char * ) const;
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
		enum ResponseStates	postUploadFile( Resources & );
		enum ResponseStates	deleteFile( Resources & );
		enum ResponseStates	deleteDir( Resources & );
		std::string	getRootPath( std::string );
		void		sendResponseHeader( enum METHODS, std::string, std::string, Resources * );
		bool		handleWriteResponse( Resources & );
		bool		isRequestReceived( Resources & ) const;
		void		setLocations( std::vector<Location> );
		void		reset( void );
	private:
		char    request[MAX_REQUEST_SIZE + 1];
		int		bytesReceived;
		int		bytesSent;
		int     fileSize;
		bool	autoIndex;
		SOCKET  socket;
		ErrorResponse   err;
		ResponseHelper	help;
		std::string		indexResponse;
		std::ifstream   file;
		std::ofstream	toUpload;
		std::string	    path;
		std::vector<Location> loc;
};