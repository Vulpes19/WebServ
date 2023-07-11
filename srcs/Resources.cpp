/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/11 15:17:02 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Resources.hpp"

Resources::Resources( void )
{
	requestLineExists = false;
	hostExists = false;
	requiredLength = -1;
	actualLength = 0; 
}

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

void	Resources::parseHeader( void )
{
	size_t colon = line.find(":");

	std::string headerKey = line.substr(0, colon);
	std::string headerValue = line.substr( colon + 2 );
	header[headerKey] = headerValue;
	if (headerKey == "Host")
		hostExists = true;
	if (headerValue.size() == 0)
		setError(BAD_REQUEST);
	if (headerKey == "Content-Length")	
		requiredLength = std::stoi(headerValue);
}

void	Resources::parseRequestLine( void )
{
	std::stringstream	ss2(line);
	std::string 		str;
	bool				isValidMethod = false;
	std::string			methods[3] = {"GET", "POST", "DELETE"};

	requestLineExists = true;
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
			setError(HTTP_VERSION_NOT_SUPPORTED);
	}
	else
		setError(BAD_REQUEST);
}

void	Resources::parseBody( void )
{
	if (requiredLength > actualLength)
	{	
		std::ofstream	requestBody("requestBody");
		fileContentBuffer += line;
		std::cout << "line: " << line << std::endl;
		fileContentBuffer += "\n";
		std::cout << "fileContentBuffer: " << fileContentBuffer << std::endl;
		actualLength = fileContentBuffer.size();
		requestBody << fileContentBuffer;
		requestBody.close();
	}
}


void    Resources::checkRequest( std::string request )
{
	std::stringstream	ss(request);
	std::string			requestBody;
	bool				requestBodyStart = false;

	std::cout << "Request" << std::endl;
	while ( std::getline(ss, line) )
	{
		std::cout << line << std::endl;
		size_t colon = line.find(":");
		if ( line == "\r" )
		{
			requestBodyStart = true;
			continue ;
		}
		else if ( requestBodyStart )
			parseBody();
		else if ( colon != std::string::npos )
			parseHeader();
		else if ( line.find("HTTP") != std::string::npos )
			parseRequestLine();
	}
	errorHandling();
	printError(getError());
}

void	Resources::errorHandling( void ) {

	if (requiredLength < actualLength - 1)
	{
		std::cout << "actual length: " << actualLength << std::endl;
		std::cout << "here 1" << std::endl;
		setError(BAD_REQUEST);
	}
	if (requiredLength == -1)
	{
		std::cout << "here 2" << std::endl;
		setError(LENGTH_REQUIRED);
	}
	if (hostExists == false || requestLineExists == false)
	{
		std::cout << "here 3" << std::endl;
		setError(BAD_REQUEST);
	}
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

void	Resources::printError(enum Error_code code) {

	//this is only for debugging purposes
	switch (code) {

		case BAD_REQUEST:
			std::cout << "bad request: " << line << std::endl;
			break ;
		case LENGTH_REQUIRED:
			std::cout << "length required" << std::endl;
			break ;
		case METHOD_NOT_ALLOWED:
			std::cout << "method not allowed" << std::endl;
			break ;
		case HTTP_VERSION_NOT_SUPPORTED:
			std::cout << "http version not supported" << std::endl;
			break ;
		default:
			std::cout << "all good" << std::endl;
			break ;
	}
}
