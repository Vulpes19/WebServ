/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/06 21:30:05 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Resources.hpp"

Resources::Resources( void )
{}

Resources::~Resources( void )
{}

Resources::Resources( const Resources &src )
{
	*this = src;
}

Resources &Resources::operator=( const Resources &rhs )
{
	if ( this != &rhs )
	{
		this->header = rhs.header;
		this->fileContentBuffer = rhs.fileContentBuffer;
		this->fileSize = rhs.fileSize;
		this->error = rhs.error;
	}
	return (*this);
}

void    Resources::checkRequest( std::string request )
{
	std::stringstream	ss(request);
	std::string			line;
	std::string			requestBody;
	size_t				requiredLength = -1;
	size_t				actualLength = 0;
	bool				requestBodyStart = false;
	std::cout << " ********************************* \n";
	while ( std::getline(ss, line) )
	{
		size_t colon = line.find(":");
		if ( line == "\r" )
		{
			requestBodyStart = true;
			continue ;
		}
		else if ( requestBodyStart )
		{
			fileContentBuffer += line;
			fileContentBuffer += "\n";
			actualLength += fileContentBuffer.size();

		}
		else if ( colon != std::string::npos )
		{
			std::string headerKey = line.substr(0, colon);
			std::string headerValue = line.substr( colon + 2 );
			header[headerKey] = headerValue;
			if (headerKey == "Content-Length")
				requiredLength = std::stoi(headerValue);
		}
		else if ( line.find("HTTP") != std::string::npos )
		{
			std::stringstream ss2(line);
			std::string str;
			ss2 >> str;
			header["Method"] = str;
			ss2 >> str;
			header["URL"] = str;
			ss2 >> str;
			header["HTTP"] = str;
		}
	}
	if (requiredLength < actualLength)
	{
		setError(BAD_REQUEST);
		std::cout << "error" << std::endl;
	}
	// for ( iterator it = header.begin(); it != header.end(); ++it )
	// {
	//     std::cout << it->first << " " << it->second << std::endl;
	// }
	// std::cout << "*********************\n";
	// std::cout << requestBody << std::endl;
	// std::cout << "*********************\n";
	// exit(1);
}

void    Resources::setError( enum Error_code error )
{
	this->error = error;
}

std::string	Resources::getRequest( std::string Key )
{
	if ( header.find(Key) != header.end() )
		return ( header[Key] );
	else
		return ( "NOT FOUND" );
}

std::string	Resources::getRequestBody( void ) const
{
	return ( fileContentBuffer );
}

void	Resources::clear( void )
{
	header.clear();
}

// enum	ResponseStates	Resources::fillFile( const char *content )
// {
// 	std::string end("\r\n\r\n");
// 	std::string tmp;
	
// 	if ( tmp.substr( tmp.length() - end.length() )  )
// 	buffer << content;
// }