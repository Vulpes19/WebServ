/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/13 12:41:27 by abaioumy         ###   ########.fr       */
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
		// this->requestBodyBuffer = rhs.requestBodyBuffer;
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

void	Resources::parseBody( size_t &size )
{
	std::cout << requiredLength << " vs " << actualLength << std::endl;
	(void)size;
	line += '\n';
	size += line.size();
	actualLength += line.size();
	requestBody.write(line.c_str(), line.size());
}



void    Resources::checkRequest( void )
{
	std::ifstream requestFile("testFile", std::ios::binary);
	
	requestBody.open("requestBody", std::ios::binary);
	if ( !requestFile.is_open() )
	{
		std::cerr << "failed to open the file\n";
		exit(1);
	}
	bool				requestBodyStart = false;
	size_t size = 0;
	char buffer[4096];
	std::string toSend;
	ssize_t bytesRead;
	while ( requestFile.read(buffer, sizeof(buffer)) )
	{
		bytesRead = requestFile.gcount();
		
		if ( !requestBodyStart )
		{
			std::string s(buffer, bytesRead);
			size_t pos = s.find("\r\n\r\n");
			if ( pos != std::string::npos )
			{
				std::string r = s.substr(pos + 4);
				if ( !r.empty() )
				{
					char *b = strstr(buffer, buffer + pos + 4);
					requestBody.write(b, r.size());
				}
				testFunc(toSend);
				requestBodyStart = true;
			}
			else
			{
				toSend += s;
			}
		}
		size += bytesRead;
		if ( requestBodyStart )
		{
			requestBody.write(buffer, bytesRead);
		}
	}
	// if ( requestFile.eof() )
	// 	std::cout << "everything is read\n";
	// if ( requestFile.fail() )
	// 	std::cout << "errorrrrrrrrrr\n";
	std::cout << "size is: " << size << std::endl;
	std::cout << "** I finished checking **\n";
	requestFile.close();
	requestBody.close();
	remove("testFile");
	errorHandling();
	// printError(getError());
}

 void    Resources::testFunc( std::string s )
{
	std::stringstream ss(s);
	while ( std::getline(ss, line) )
	{
		size_t colon = line.find(":");
		if ( colon != std::string::npos )
			parseHeader();
		else if ( line.find("HTTP") != std::string::npos )
			parseRequestLine();
	}
}
// void    Resources::checkRequest( void )
// {
// 	std::ifstream requestFile("testFile", std::ios::binary);
	
// 	size_t size = 0;
// 	requestBody.open("requestBody");
// 	if ( !requestFile.is_open() )
// 	{
// 		std::cerr << "failed to open the file\n";
// 		exit(1);
// 	}
// 	bool				requestBodyStart = false;

// 	while ( std::getline(requestFile, line) )
// 	{
// 		size_t colon = line.find(":");
// 		if ( line == "\r" )
// 		{
// 			requestBodyStart = true;
// 			continue ;
// 		}
// 		else if ( requestBodyStart )
// 			parseBody(size);
// 		else if ( colon != std::string::npos )
// 			parseHeader();
// 		else if ( line.find("HTTP") != std::string::npos )
// 			parseRequestLine();
// 	}
// 	std::cout << "size is: " << size << std::endl;
// 	std::cout << "** I finished checking **\n";
// 	requestFile.close();
// 	requestBody.close();
// 	remove("testFile");
// 	errorHandling();
// 	// printError(getError());
// }

void	Resources::errorHandling( void ) {

	if (requiredLength < actualLength)
		setError(BAD_REQUEST);
	if (requiredLength == -1)
		setError(LENGTH_REQUIRED);
	if (hostExists == false || requestLineExists == false)
		setError(BAD_REQUEST);
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

// std::ofstream	Resources::getRequestBody( void ) const
// {
// 	return ( requestBodyBuffer );
// }

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

// enum	ResponseStates	Resources::fillFile( const char *content )
// {
// 	std::string end("\r\n\r\n");
// 	std::string tmp;
	
// 	if ( tmp.substr( tmp.length() - end.length() )  )
// 	buffer << content;
// }
