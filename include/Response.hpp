/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:59 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/18 12:56:14 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"
#include "ClientManager.hpp"
#include "Resources.hpp"

class ClientManager;

struct ResponseHelper
{
	ssize_t     		getFileSize( const char * ) const;
	bool				isDirectory( std::string ) const;
	void				normalizePath( std::string & );
	const std::string	getFileType( std::string, enum TYPES ) const;
	const std::string	getCurrentTime( void ) const;
	const std::string	getFileLocation( const char * ) const;
	bool				getAutoIndex( std::vector<Location> &, std::string path ) const;
	const redir			checkForRedirections( std::vector<Location> &, std::string path ) const;
};

struct ErrorResponse
{
	ResponseHelper help;
	void        errorBadRequest( SOCKET, std::string ); //400
	void        errorNotFound( SOCKET, std::string ); //404
	void    	errorForbidden( SOCKET, std::string ); //403
	void    	errorInternal( SOCKET, std::string ); //500
	void    	errorUnauthorized( SOCKET, std::string ); //401
	void    	errorMethodNotAllowed( SOCKET, std::string ); //405
	void    	errorLengthRequired( SOCKET, std::string ); //411
	void    	errorHTTPVersion( SOCKET, std::string ); //505
	void		errorRequestTooLarge( SOCKET, std::string ); //413
};



class Response
{
	public:
		Response( void );
		~Response( void );
		void        setSocket( SOCKET );
		enum ResponseStates	handleReadRequest( Resources &, std::string &serverName );
		enum ResponseStates	getResponseFile( std::string );
		enum ResponseStates	getResponseDir( std::string );
		enum ResponseStates	postUploadFile( Resources & );
		enum ResponseStates	deleteFile( std::string, Resources & );
		enum ResponseStates	deleteDir( Resources & );
		std::string	getRootPath( std::string );
		std::string	getUploadPath( std::string );
		void		sendResponseHeader( enum METHODS, std::string, std::string, Resources * );
		bool		handleWriteResponse( Resources & );
		bool		handleErrors( Resources & );
		bool		isRequestReceived( std::string, ssize_t ) const;
		void		handleRedirection( redir & );
		void		setLocations( std::vector<Location> );
		void		setErrorPages( std::map< std::string, std::string > );
		void		setName( std::string );
		void		setHost( std::string );
		void		setPort( std::string );
        void		setBodySize( ssize_t );
		void		reset( void );
	private:
		std::vector< Location >				loc;
		std::map< std::string, std::string > errorPages;
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
		std::string		serverName;
		std::string		port;
		std::string		host;
		std::string		uploadPath;
		std::ofstream	buffer;
		ssize_t			bodySize;
		ssize_t			bodyLimit;
		bool			isBody;
};