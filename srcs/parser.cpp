/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/11 19:10:10 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "configData.hpp"

Parser::Parser(): openingBraceCount(0), host_exists(false), closingBraceExpected(true), status(OK) {};

Parser::~Parser() {};

void    Parser::openFile(char *argv) {

	if (!argv)
		std::cout << "ERROR: File doesn't exist or could not be open!" << std::endl;
	else
	{
		std::ifstream   confFile(argv);
		readFile(confFile);
	}
} ;

bool    Parser::checkBracesError() {

	if (openingBraceCount == 0)
		return true;
	else
		return false;
}

void	Parser::setServerContent(ServerSettings &server, int which, std::string value) {

	switch (which) {

		case PORT:
			if (host_exists == false)
			{
				if (value.size() - 1 > 0) {
					if (findSemicolon(value))
						server.setPort(value.erase(value.size() - 1));
					else
						printError(SEMICOLON);
				}
				else
					printError(EMPTY);
			}
			else
				server.setPort(value);
			break ;
		case HOST:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))
					server.setHost(value.erase(value.size() - 1));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case NAME:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))    
					server.setName(value.erase(value.size() - 1));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case SIZE:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))
					server.setSize(stoi(value));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case ERROR_PAGE:
			std::stringstream ss(value);
			std::string directive, status_code, path;
			
			ss >> directive >> status_code >> path;
			if (value.size() - 1 > 0) {
				if (findSemicolon(path)) {
					ErrorPage	error_page;
					
					error_page.setStatusCode(stoi(status_code));
					error_page.setPath(path.erase(path.size() - 1));
					server.setErrorPages(error_page);
				}
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
	}
}

void	Parser::setLocationContent(Location& location, int which, std::string value) {

	switch (which) {

		case ROOT:
			if (value.size() - 1 > 0) {	
				if (findSemicolon(value))
					location.setRoot(value.erase(value.size() - 1));
				else
					printError(SEMICOLON);
			}	
			else
				printError(EMPTY);
			break ;
		case INDEX:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))	
					location.setIndex(value.erase(value.size() - 1));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case AUTOINDEX:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))
					location.setAutoIndex();
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case UPLOAD:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))
					location.setUpload(value.erase(value.size() - 1));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case RETURN:
			if (value.size() - 1 > 0) {
				if (findSemicolon(value))
					location.setValue(value.erase(value.size() - 1));
				else
					printError(SEMICOLON);
			}
			break ;
	}
}

void   Parser::readFile(std::ifstream& confFile) {

	// bool foundServer = false;
	// bool openingBrace = false;
	// std::string buffer;
	if (confFile.is_open())
	{
		while (getline(confFile, line))
		{
			// buffer += line;
			// buffer += "\n";
			// if (buffer.find("server") != std::string::npos)
			// 	foundServer = true;
			// if (buffer.find("{") != std::string::npos && foundServer)
			// {	
			// 	std::cout << "start parsing" << std::endl;
			// 	break ;
			// }
			std::stringstream ss(line);
			ss >> directive >> value;
			std::cout << "directive: " << directive << std::endl;
			std::cout << "value: " << value << std::endl;
			std::cout << "line: " << line << std::endl;
			if ((directive == "server" && value == "{" )|| line == "{") {
				parseServer(confFile);
			}
		}
		// std::cout << buffer << std::endl;
	}
	else
		std::cout << "Error: could not open the configuration file!" << std::endl;
} ;

void	isPort(std::string value) {

	std::istringstream ss(value);
	int	res;

	ss >> res;
	std::cout << res << std::endl;
}

void	Parser::parseServer(std::ifstream& confFile) {

	ServerSettings  server;
	Context context;
	std::string optionalVal;

	while (getline(confFile, line)) {

		if (line[0] == '#' || line.empty())
			continue ;
		std::stringstream ss(line);
		ss >> directive >> value >> optionalVal;
		if (directive == "listen")
		{
			if (optionalVal.size())
				host_exists = true;
			setServerContent(server, PORT, value);
			if (host_exists)
				setServerContent(server, HOST, optionalVal);
		}
		else if (directive == "server_name")
			setServerContent(server, NAME, value);
		else if (directive == "body_size")
			setServerContent(server, SIZE, value);
		else if (directive == "error_page")
			setServerContent(server, ERROR_PAGE, line);
		else if (directive == "location") {
			parseLocation(confFile, server, value);              
		}
		else if (directive == "}" && closingBraceExpected) {
				context.setServer(server);
				parsedData.push_back(context);
				break ;
		}
	}
}

void	Parser::parseLocation(std::ifstream& confFile, ServerSettings& server, std::string& value) {

	Location	location;

	location.setValue(value);
	while(getline(confFile, line) && status == OK) {

		if (line[0] == '#' || line.empty())
			continue ;
		std::stringstream ss(line);
		ss >> directive >> value;
		if (directive == "}" && closingBraceExpected) {
			server.setLocations(location);
			break ;
		}
		if (directive == "root")
			setLocationContent(location, ROOT, value);
		else if (directive == "index")
			setLocationContent(location, INDEX, value);
		else if (directive == "autoindex")
			setLocationContent(location, AUTOINDEX, value);
		else if (directive == "upload")
			setLocationContent(location, UPLOAD, value);
		else if (directive == "return")
			setLocationContent(location, RETURN, value);
	}
}

void	Parser::printData() {

	for (size_t i = 0; i < parsedData.size(); i++) {

		Context context;
		context = parsedData[i];

		ServerSettings  server;
		server = context.getServer();

		std::cout << std::endl;
		std::cout << "ServerSettings: " << std::endl;
		std::cout << "listen: " << server.getPort() << std::endl;
		std::cout << "host: " << server.getHost() << std::endl;
		std::cout << "name: " << server.getName() << std::endl;
		std::cout << "body size: " << server.getSize() << std::endl;
		
		std::vector<ErrorPage>	epVec = server.getErrorPages();
		for (size_t i = 0; i < epVec.size(); i++) {
			
			std::cout << "error_pages: " << std::endl;
			std::cout << "status code: " << epVec[i].getStatusCode() << std::endl;
			std::cout << "path: " << epVec[i].getPath() << std::endl;
		}
		std::vector<Location>   locationVec = server.getLocations();
		for (size_t i = 0; i < locationVec.size(); i++) {

			std::cout << "locations: " << std::endl;
			std::cout << "value: " << locationVec[i].getValue() << std::endl;
			std::cout << "root: " << locationVec[i].getRoot() << std::endl;
			std::cout << "index: " << locationVec[i].getIndex() << std::endl;
			std::cout << "upload: " << locationVec[i].getUpload() << std::endl;
			if (locationVec[i].getAutoIndex() == ON)
				std::cout << "autoindex: on" << std::endl;
		}
	}
}

bool    Parser::findSemicolon(std::string value) {

	size_t  pos = value.find(";") ;

	if (pos  != std::string::npos)
		return (true);
	return (false);
}

void    Parser::printError(int which) {

	status = ERROR;
	switch(which) {

		case SEMICOLON:
			std::cout << "Error: could not find semicolon" << std::endl;
			break ;
		case CURLYBRACE:
			std::cout << "Error: could not find curly brace" << std::endl;
			break ;
		case UNKNOWN:
			std::cout << "Error: " << directive << " <- Unknown expression." << std::endl;
			break ;
		case EMPTY:
			std::cout << "Error: " << directive << " <- Directive can't have empty value!" << std::endl;
			break ;
	}
	exit(1);
}

std::vector<Context> Parser::getParsedData( void ) const
{
	return (parsedData);
}