/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 13:52:42 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/03 15:03:02 by abaioumy         ###   ########.fr       */
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
	FILE_IO_ERROR
};

class Resources
{
	public:
		typedef std::map< std::string, std::string >::iterator iterator;
		Resources( void );
		~Resources( void );
		Resources( const Resources &src );
		Resources &operator=( const Resources &rhs );
		void	checkRequest( std::string request );
		void	setError( enum Error_code error );
		void	setResponseHeader( void );
	private:
		std::map< std::string, std::string > header;
		const char *fileContentBuffer;
		int fileSize;
		enum Error_code error;
};