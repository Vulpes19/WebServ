/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 13:52:42 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/07 17:14:32 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
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
	LENGTH_REQUIRED
};

class Resources
{
	public:
		typedef std::map< std::string, std::string >::iterator iterator;
		Resources( void );
		~Resources( void );
		Resources( const Resources &src );
		Resources &operator=( const Resources &rhs );
		void		checkRequest( std::string request );
		void		setError( enum Error_code error );
		// enum ResponseStates	fillFile( const char * );
		std::string	getRequest( std::string Key );
		std::string	getRequestBody( void ) const;
		enum Error_code	getError() const;
		void		clear( void );
		void		errorHandling( void );
	private:
		std::map< std::string, std::string > header;
		enum Error_code error;
		// std::fstream	buffer;
		std::string	fileContentBuffer;
		int			fileSize;
		size_t				requiredLength;
		size_t				actualLength;
};