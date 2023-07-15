/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/15 14:43:34 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Resources.hpp"

Resources::Resources( void )
{
	requestLineExists = false;
	hostExists = false;
	requiredLength = -1;
	actualLength = 0;
	isPost = false;
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

bool	Resources::methodValidity(std::string value) {
	
	std::string			methods[3] = {"GET", "POST", "DELETE"};

	if (value == "POST") {

		isPost = true;
		return (true);
	}
	for (int i = 0; i < 3; i++) {
		if (methods[i] == value) {
			return (true);
			break ;
		}
	}
	return (false);
}

void	Resources::parseRequestLine( void )
{
	std::stringstream	ss2(line);
	std::string 		str;

	requestLineExists = true;
	ss2 >> str;
	if (methodValidity(str))
		header["Method"] = str;
	else
		setError(METHOD_NOT_ALLOWED);
	ss2 >> str;
	if (str.size())
		header["URL"] = str;
	else
	{
		std::cout << "here" << std::endl;
		setError(BAD_REQUEST);
	}
	ss2 >> str;
	if (str.size()) {
		if (str == "HTTP/1.1")
			header["HTTP"] = str;
		else
			setError(HTTP_VERSION_NOT_SUPPORTED);
	}
	else
	{
		std::cout << "here 1" << std::endl;
		setError(BAD_REQUEST);
	}
}

void	Resources::parseBody( size_t &size )
{
	size += line.size();
	actualLength += line.size();
	requestBody.write(line.c_str(), line.size());
}

void    Resources::checkRequest( void )
{
	requestBody.open("requestBody");
	std::ifstream requestFile("readingRequestFile", std::ios::binary);
	size_t size = 0;
	if ( !requestFile.is_open() )
	{
		std::cerr << "failed to open the file\n";
		exit(1);
	}
	bool				requestBodyStart = false;

	while ( std::getline(requestFile, line) )
	{
		if (!requestFile.eof() && requestBodyStart )
			line += '\n';
		size_t colon = line.find(":");
		if ( line == "\r" )
		{
			requestBodyStart = true;
			continue ;
		}
		else if ( requestBodyStart )
			parseBody(size);
		else if ( colon != std::string::npos )
			parseHeader();
		else if ( line.find("HTTP") != std::string::npos )
			parseRequestLine();
	}
	requestFile.close();
	requestBody.close();
	remove("readingRequestFile");
	errorHandling();
	printError(getError());
}

void	Resources::errorHandling( void ) {

	if (requiredLength < actualLength - 1)
		setError(REQUEST_ENTITY_TOO_LARGE);
	if (requiredLength == -1 && isPost)
		setError(LENGTH_REQUIRED);
	if (hostExists == false || requestLineExists == false)
	{
		std::cout << "here 2" << std::endl;
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

void	Resources::clear( void )
{
	header.clear();
}

void	Resources::printError(enum Error_code code) {

	//this is only for debugging purposes
	switch (code) {

		case BAD_REQUEST:
			std::cout << "BAD REQUEST!" << std::endl;
			break ;
		case LENGTH_REQUIRED:
			std::cout << "LENGTH REQUIRED!" << std::endl;
			break ;
		case METHOD_NOT_ALLOWED:
			std::cout << "METHOD NOT ALLOWED" << std::endl;
			break ;
		case HTTP_VERSION_NOT_SUPPORTED:
			std::cout << "HTTP VERSION NOT SUPPORTED" << std::endl;
			break ;
		default:
			std::cout << "all good" << std::endl;
			break ;
	}
}
