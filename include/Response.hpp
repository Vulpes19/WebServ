/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/15 10:31:20 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "ClientManager.hpp"
#include "Resources.hpp"

class ClientManager;

struct ErrorResponse
{
	void        errorBadRequest( SOCKET ); //400
	void        errorNotFound( SOCKET ); //404
	void    	errorForbidden( SOCKET ); //403
	void    	errorInternal( SOCKET ); //500
	void    	errorUnauthorized( SOCKET ); //401
	void    	errorMethodNotAllowed( SOCKET ); //405
	void    	errorUnsupportedMediaType( SOCKET ); //415
	void    	errorTimeout( SOCKET ); //504
	void    	errorLengthRequired( SOCKET ); //411
	void    	errorHTTPVersion( SOCKET ); //505
	void		errorRequestTooLarge( SOCKET ); //413
};

struct ResponseHelper
{
	ssize_t     getFileSize( const char * ) const;
	bool		isDirectory( std::string ) const;
	void		normalizePath( std::string & );
	const std::string	getFileType( std::string, enum TYPES ) const;
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
		std::string	getUploadPath( std::string );
		void		sendResponseHeader( enum METHODS, std::string, std::string, Resources * );
		bool		handleWriteResponse( Resources & );
		bool		handleErrors( Resources & );
		bool		isRequestReceived( std::string, ssize_t ) const;
		void		setLocations( std::vector<Location> );
		void		setName( std::string );
		void		setHost( std::string );
        void		setBodySize( ssize_t );
		void		reset( void );
	private:
		// char    request[MAX_REQUEST_SIZE + 1];
		ssize_t			bytesReceived;
		ssize_t			bytesSent;
		int     		fileSize;
		bool			autoIndex;
		SOCKET  		socket;
		ErrorResponse   err;
		ResponseHelper	help;
		std::string		indexResponse;
		std::ifstream   file;
		std::ofstream	toUpload;
		std::string	    path;
		std::string		serverName;
		std::string		host;
		std::string		uploadPath;
		std::vector<Location> loc;
		std::ofstream	buffer;
		ssize_t			bodySize;
		ssize_t			bodyLimit;
		bool			isBody;
};