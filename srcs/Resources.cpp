/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elias <elias@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/11 16:48:13 by elias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Resources.hpp"

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
			requestBody += line;
			requestBody += "\n";
		}
		else if ( colon != std::string::npos )
		{
			std::string headerKey = line.substr(0, colon);
			std::string headerValue = line.substr( colon + 2 );
			header[headerKey] = headerValue;
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

	std::cout << "*#########################\n";
	std::map<std::string, std::string>::iterator it;
	for (it = header.begin(); it != header.end(); it++)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}

	std::cout << "*#########################\n";

	
	fileContentBuffer = requestBody.c_str();
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

void    Resources::setResponseHeader( void )
{
	
}

std::string	&Resources::getRequest( std::string Key )
{
	return ( header[Key] );
}