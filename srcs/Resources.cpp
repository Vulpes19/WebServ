/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/07 17:15:05 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Resources.hpp"

Resources::Resources( void ): actualLength(0), requiredLength(-1)
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
		this->actualLength = rhs.actualLength;
		this->requiredLength = rhs.requiredLength;
	}
	return (*this);
}

void    Resources::checkRequest( std::string request )
{
	std::stringstream	ss(request);
	std::string			line;
	std::string			requestBody;
	bool				requestBodyStart = false;
	bool				isValidMethod = false;
	std::string			methods[3] = {"GET", "POST", "DELETE"};
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
			if (headerValue.size() == 0)
				setError(BAD_REQUEST);
			if (headerKey == "Content-Length")	
				requiredLength = std::stoi(headerValue);
		}
		else if ( line.find("HTTP") != std::string::npos )
		{
			std::stringstream ss2(line);
			std::string str;
			ss2 >> str;
			for (int i = 0; i < 3; i++) {
				if (methods[i] == str) {
					isValidMethod = true;
					break ;
				}
			}
			if (isValidMethod)
				header["Method"] = str;
			else
				setError(METHOD_NOT_ALLOWED);
			ss2 >> str;
			if (str.size())
				header["URL"] = str;
			else
				setError(BAD_REQUEST);
			ss2 >> str;
			if (str.size()) {
				if (str == "HTTP/1.1")
					header["HTTP"] = str;
				else
					setError(BAD_REQUEST);
			}
			else
				setError(BAD_REQUEST);
		}
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

void	Resources::errorHandling( void ) {

	if (requiredLength < actualLength)
		setError(BAD_REQUEST);
	if (requiredLength == -1)
		setError(LENGTH_REQUIRED);
}

void    Resources::setError( enum Error_code error )
{
	this->error = error;
}

enum Error_code	Resources::getError() const {

	return (error);
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
