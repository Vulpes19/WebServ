/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/04 20:50:17 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parser.hpp"
#include "include/configData.hpp"

Parser::Parser(): openingBraceCount(0), closingBraceExpected(false), status(OK) {};

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
		return false;
	else
		return true;
}

void	Parser::setServerContent(Server &server, int which, std::string value) {

	switch (which) {

		case PORT:
			if (findSemicolon(value))
				server.setPort(value.erase(value.size() - 1));
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1) ;
			}
			break ;
		case NAME:
			if (findSemicolon(value))    
				server.setName(value.erase(value.size() - 1));
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1) ;
			}
			break ;
		case SIZE:
			if (findSemicolon(value))
				server.setSize(stoi(value));
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1);
			}
			break ;
	}
}

void	Parser::setLocationContent(Location& location, int which, std::string value) {

	switch (which) {

		case ROOT:
			if (findSemicolon(value))
				location.setRoot(value.erase(value.size() - 1));
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1) ;
			}
			break ;
		case INDEX:
			if (findSemicolon(value))	
				location.setIndex(value.erase(value.size() - 1));
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1) ;
			}
			break ;
		case AUTOINDEX:
			if (findSemicolon(value))
				location.setAutoIndex();
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1) ;
			}
			break ;
		case UPLOAD:
			if (findSemicolon(value))
				location.setUpload(value.erase(value.size() - 1));
			else {
				printError(SEMICOLON);
				status = ERROR;
				exit(1) ;
			}
			break ;
	}
}

void   Parser::readFile(std::ifstream& confFile) {

	if (confFile.is_open())
	{
		while (getline(confFile, line))
		{
			std::stringstream ss(line);
			ss >> directive >> value;
			if (directive == "server" && value == "{") {

				openingBraceCount++;
				parseServer(confFile);
			}
		}
	}
	else
		std::cout << "Error: could not open the configuration file!" << std::endl;
} ;

void	Parser::parseServer(std::ifstream& confFile) {

	Server  server;
	Context context;
	std::string brace;

	while (getline(confFile, line) && status == OK) {

		if (line[0] == '#' || line.empty())
			continue ;
		std::stringstream ss(line);
		ss >> directive >> value >> brace;
		if (directive == "listen")
			setServerContent(server, PORT, value);
		else if (directive == "server_name")
			setServerContent(server, NAME, value);
		else if (directive == "body_size")
			setServerContent(server, SIZE, value);
		else if (directive == "location") {
			openingBraceCount++;
			parseLocation(confFile, server, value);                
		}
		if (line[0] == '}') {

			openingBraceCount--;
			if (openingBraceCount == 0) {
				context.setServer(server);
				parsedData.push_back(context);
				break ;
			}
			else
				std::cout << "server brace error" << std::endl;
		}
	}
}

void	Parser::parseLocation(std::ifstream& confFile, Server& server, std::string& value) {

	Location    location;

	location.setValue(value);
	while(getline(confFile, line) && status == OK) {

		std::stringstream ss(line);
		ss >> directive >> value;
		if (directive == "}") {
			openingBraceCount--;
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
	}
}

void	Parser::printData() {

	for (int i = 0; i < parsedData.size(); i++) {

		Context context;
		context = parsedData[i];

		Server  server;
		server = context.getServer();

		std::cout << std::endl;
		std::cout << "Server: " << std::endl;
		std::cout << "listen: " << server.getPort() << std::endl;
		std::cout << "name: " << server.getName() << std::endl;
		std::cout << "body size: " << server.getSize() << std::endl;

		std::vector<Location>   locationVec = server.getLocations();
		for (int i = 0; i < locationVec.size(); i++) {

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

	switch(which) {

		case SEMICOLON:
			std::cout << "Error: could not find semicolon" << std::endl;
			std::cout << "line: " << directive << " " << value << "<-" << std::endl;
			break ;
		case CURLYBRACE:
			std::cout << "Error: could not find curly brace" << std::endl;
			break ;
	}
}