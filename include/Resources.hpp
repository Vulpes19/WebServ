/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 13:52:42 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/09 15:00:09 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

enum Error_code
{
	NO_ERROR,
	BAD_REQUEST,
	NOT_FOUND,
	INTERNAL_SERVER_ERROR,
	UNAUTHORIZED,
	FORBIDDEN,
	METHOD_NOT_ALLOWED,
	UNSOPPORTED_MEDIA_TYPE,
	REQUEST_TIMEOUT,
	FILE_IO_ERROR,
	LENGTH_REQUIRED,
	HTTP_VERSION_NOT_SUPPORTED
};

class Resources
{
	public:
		typedef std::map< std::string, std::string >::iterator iterator;
		Resources( void );
		~Resources( void );
		Resources( const Resources &src );
		Resources &operator=( const Resources &rhs );
		void		checkRequest( void );
		void		setError( enum Error_code error );
		// enum ResponseStates	fillFile( const char * );
		std::string	getRequest( std::string Key );
		std::string	getRequestBody( void ) const;
		enum Error_code	getError() const;
		void		clear( void );
		void		errorHandling( void );
		void		parseBody( void );
		void		parseRequestLine( void );
		void    	parseHeader( void );
		void    	printError(enum Error_code code);
	private:
		std::map< std::string, std::string > header;
		enum Error_code error;
		// std::fstream	buffer;
		std::string	fileContentBuffer;
		int			fileSize;
		ssize_t				requiredLength;
		ssize_t				actualLength;
		bool 				hostExists;
		std::string			line;
		bool 				requestLineExists;
};