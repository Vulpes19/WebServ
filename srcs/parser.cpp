/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/26 15:37:43 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "configData.hpp"

Parser::Parser(): openingBraceCount(0), host_exists(false), status(OK), uploadExists(false) {};

Parser::~Parser() {
	parsedData.clear();
};

int		countWords(std::string str) {

	std::stringstream ss(str);
	int	words = 0;
	std::string	word;

	while (ss >> word)
		words++;
	return (words);
}

bool	findMethod(std::string value) {

	std::string	methods[] = {"GET", "POST", "DELETE"};

	for (int i = 0; i < 3; i++) {

		if (value.find(methods[i]) != std::string::npos)
			return (true);
	}
	return (false);
}

bool	testExtension(char *argv) {

	std::string str(argv);

	if (str.find(".conf") != std::string::npos)
		return (false);
	return (true);
}

void    Parser::openFile(char *argv) {

	if (!argv)
		printError(NO_CONFIG_FILE);
	else if (testExtension(argv))
		printError(EXTENSION);
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

std::string	Parser::cleanValue(std::string value) {

	size_t	pos = value.find_first_of(';');

	while (pos != std::string::npos) {
		value.erase(pos, 1);
		pos = value.find_first_of(';', pos);
	}
	return (value);
}

void	Parser::setServerContent(ServerSettings &server, int which, std::string value) {

	switch (which) {

		case PORT:
			if (host_exists == false)
			{
				if (value.size() - 1 > 0) {
					if (findSemicolon())
						server.setPort(cleanValue(value));
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
				if (findSemicolon())
					server.setHost(cleanValue(value));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case NAME:
			if (value.size() - 1 > 0) {
				if (findSemicolon())    
					server.setName(cleanValue(value));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case SIZE:
			if (value.size() - 1 > 0) {
				if (findSemicolon()) {
					std::stringstream ss(value);
					ssize_t size;
					ss >> size;
					server.setSize(size);
				}
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case UPLOAD:
			if (value.size() - 1 > 0) {
				if (findSemicolon()) {   
					server.setUpload(cleanValue(value));
					uploadExists = true;
				}
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
			if (countWords(value) != 3)
				printError(EMPTY);
			if (status_code.size() != 3)
				printError(INVALID_STATUS_CODE);
			if (path.find("/") == std::string::npos)
				printError(INVALID_PATH);
			if (value.size() - 1 > 0) {
				if (findSemicolon())
					server.setErrorPages(status_code, cleanValue(path));
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
			if (value.find("/") == std::string::npos)
				printError(INVALID_PATH);
			if (value.size() - 1 > 0) {	
				if (findSemicolon())
					location.setRoot(cleanValue(value));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case INDEX:
			if (value.size() - 1 > 0) {
				if (findSemicolon())
					location.setIndex(cleanValue(value));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case AUTOINDEX:
			if (value.size() - 1 > 0) {
				if (findSemicolon() && value.find("on") != std::string::npos)	
					location.setAutoIndex();
				else if (findSemicolon() && value.find("off") != std::string::npos)
					break ;
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case UPLOAD:
			if (value.size() - 1 > 0) {
				if (findSemicolon())
					location.setUpload(cleanValue(value));
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case CGI:
			if (value.find("/") == std::string::npos)
				printError(INVALID_PATH);
			if (value.size() - 1 > 0) {
				if (findSemicolon()) {
					location.setCGIbool();
					location.setCGI(cleanValue(value));
				}
				else
					printError(SEMICOLON);
			}
			else
				printError(EMPTY);
			break ;
		case METHOD:
			if (findSemicolon()) {
				std::stringstream ss(value);
				std::string method;
				
				while (ss >> method) {
					if (findMethod(cleanValue(method)))
						location.setAllowedMethods(cleanValue(method));
				}
			}
			else
				printError(SEMICOLON);
			break ;
		case RETURN:
			std::stringstream ss(value);
			std::string path;
			std::string	str;
			std::string	status_code;

			if (countWords(value) == 2) {
				ss >> str >> status_code;
				if (status_code.size() - 1 > 0) {
					if (findSemicolon())
						location.setRedirection(cleanValue(status_code), "");
					else
						printError(SEMICOLON);				
				}
				else
					printError(EMPTY);
			}
			else if (countWords(value) == 3) {	
				ss >> str >> status_code >> path;
				if (path.find("/") == std::string::npos)
					printError(INVALID_PATH);
				if (path.size() - 1 > 0) {
					if (findSemicolon())
						location.setRedirection(status_code, cleanValue(path));
					else
						printError(SEMICOLON);				
				}
				else
					printError(EMPTY);
			}
			else if (countWords(value) > 3) {
				throw excp("Syntax Error: Return field is invalid, too many elements!");
				// std::cerr << "Syntax Error: Return field is invalid, too many elements!" << std::endl;
				// exit(1);
			}
			else {
				printError(EMPTY);
				break ;
			}
			break ;
	}
}

void   Parser::readFile(std::ifstream& confFile) {

	bool	isEmpty = true;
	if (confFile.is_open())
	{
		while (getline(confFile, line))
		{
			if (line.find("#") != std::string::npos || line.empty())
				continue ;
			if (line.find("{") != std::string::npos)
				openingBraceCount++;
			else if (line.find("}") != std::string::npos)
				openingBraceCount--;
			if (line.find("server") != std::string::npos){
				isEmpty = false;
				parseServer(confFile);
			}
		}
	}
	else
		printError(NO_CONFIG_FILE);
	if (checkBracesError())
		printError(CURLYBRACE);
	if (isEmpty)
		printError(FILE_EMPTY);
} ;

void	Parser::serverValuesValidation(ServerSettings server) {

	std::string port = server.getPort();
	int			portInt;
	std::stringstream ss(port);

	ss >> portInt;
	if (port.find(":") != std::string::npos || port.find(".") != std::string::npos) {
		throw excp("Syntax Error: port is invalid!");
	}	
	if (port.size() == 0) {
		throw excp("Syntax Error: port not found!");
	}
	if (server.getName().size() == 0) {	
		throw excp("Syntax Error: server name not found!");
	}
	if (portInt == 0) {	
		throw excp("Syntax Error: port is invalid!");
	}
	if (server.getLocations().size() == 0) {
		throw excp("Syntax Error: Location is undefined!");
	}
}

void	Parser::parseServer(std::ifstream& confFile) {

	ServerSettings  server;
	Context context;
	std::string optionalVal;

	server.initErrorPages();
	while (getline(confFile, line)) {

		std::stringstream ss(line);
		ss >> directive >> value >> optionalVal;
		if (line.find("#") != std::string::npos || line.empty())
			continue ;
		if (line.find("{") != std::string::npos)
			openingBraceCount++;
		if (line.find("listen") != std::string::npos)
		{
			if (optionalVal.size())
				host_exists = true;
			setServerContent(server, PORT, value);
			if (host_exists)
				setServerContent(server, HOST, optionalVal);
		}
		else if (line.find("upload") != std::string::npos)
			setServerContent(server, UPLOAD, value);
		else if (line.find("server_name") != std::string::npos)
			setServerContent(server, NAME, value);
		else if (line.find("body_size") != std::string::npos)
			setServerContent(server, SIZE, value);
		else if (line.find("error_page") != std::string::npos)
			setServerContent(server, ERROR_PAGE, line);
		else if (line.find("location") != std::string::npos)
			parseLocation(confFile, server, value);              
		else if (line.find("}") != std::string::npos) {
			openingBraceCount--;
			context.setServer(server);
			parsedData.push_back(context);
			break ;
		}
	}
	serverValuesValidation(server);
}

bool	examinePath(std::string value) {

	if (value.find("/") != std::string::npos)
		return (true);
	return (false);
}

void	Parser::locationValuesValidation(Location location) {

	if (location.getValue().size() == 0 || !examinePath(location.getValue())) {
		throw excp("Syntax Error: location value not found or invalid!");
	}
	if (location.getRoot().size() == 0 || !examinePath(location.getRoot())) {
		throw excp("Syntax Error: location root value not found or invalid!"); 
	}
	if (location.getIndex().size() == 0) {
		location.setIndex("homepage.html");
	}
	if (location.getIndex().find(".") == std::string::npos) {
		throw excp("Syntax Error: index value is invalid, index value must contain an extension!");
	}
	if ((location.getRedirection().status_code.size() > 3 || location.getRedirection().status_code.size() < 3) && 
		location.getRedirection().status_code != "-1") {
		throw excp("Syntax Error: return status code is invalid!");
	}
	if (location.getCGIbool() && location.getCGI().empty()) {
		throw excp("Syntax Error: CGI script not defined or invalid!");
	}
	if (location.getCGIbool() && !examinePath(location.getCGI())) {
		throw excp("Syntax Error: CGI script path is invalid or not defined!");
	}
	if (location.getCGIbool() && (location.getRoot().empty() || !examinePath(location.getRoot()))) {
		throw excp("Syntax Error: CGI root path is invalid or not defined!");
	}
	std::stringstream ss(location.getRedirection().status_code);
	int				  code;
	
	ss >> code;
	if (code == 0) {
		throw excp("Syntax Error: return status code is invalid!");
	}
}

bool	Parser::examineLocation() {

	if (value == "/")
		return (true);
	else if (value[0] == '/' && value[value.size() - 1] == '/')
		return (true);
	status = ERROR;
	return (false);
}

void	Parser::parseLocation(std::ifstream& confFile, ServerSettings& server, std::string& value) {

	Location	location;
	
	if (examineLocation()) {
		if (value == "/cgi-bin/")
			location.setCGIbool();
		location.setValue(value);
	}
	while(getline(confFile, line) && status == OK) {

		std::stringstream ss(line);
		ss >> directive >> value;
		if (line.find("#") != std::string::npos || line.empty())
			continue ;
		if (line.find("{") != std::string::npos)
			openingBraceCount++;
		if (directive == "root")
			setLocationContent(location, ROOT, value);
		else if (directive == "index")
			setLocationContent(location, INDEX, value);
		else if (directive == "autoindex")
			setLocationContent(location, AUTOINDEX, value);
		else if (directive == "upload" && uploadExists == false)
			setLocationContent(location, UPLOAD, value);
		else if (directive == "return")
			setLocationContent(location, RETURN, line);
		else if ((directive == "cgi" || directive == "CGI"))
			setLocationContent(location, CGI, value);
		else if (findMethod(directive))
			setLocationContent(location, METHOD, line);
		if (line[0] == '}') {
			openingBraceCount--;
			server.setLocations(location);
			break ;
		}
		else if (line.find("}") != std::string::npos) {
			openingBraceCount--;
			server.setLocations(location);
			break ;
		}
	}
	locationValuesValidation(location);
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
		std::cout << "upload: " << server.getUpload() << std::endl;
		
		// std::cout << "Errors: " << std::endl;
		// std::map<std::string, std::string> epMap = server.getErrorPages();
		// std::map<std::string, std::string>::iterator itr;
		// for(itr=epMap.begin();itr!=epMap.end();itr++)
		// {
		// 	std::cout << itr->first << " " << itr->second << std::endl;
		// }
		std::vector<Location>   locationVec = server.getLocations();
		for (size_t i = 0; i < locationVec.size(); i++) {

			std::cout << "locations: " << std::endl;
			
			std::vector<std::string>	allowedMethods = locationVec[i].getAllowedMethods();
			std::cout << "allowed methods: " << std::endl;
			std::cout << "*******" << std::endl;
			for (size_t i = 0; i < allowedMethods.size(); i++)
				std::cout << allowedMethods[i] << " ";
			std::cout << "*******" << std::endl;
			std::cout << "value: " << locationVec[i].getValue() << std::endl;
			std::cout << "root: " << locationVec[i].getRoot() << std::endl;
			if (locationVec[i].getCGIbool() == true)
				std::cout << "CGI: " << locationVec[i].getCGI() << std::endl;
			std::cout << "index: " << locationVec[i].getIndex() << std::endl;
			std::cout << "upload: " << locationVec[i].getUpload() << std::endl;
			std::cout << "return: " << locationVec[i].getRedirection().status_code; 
			std::cout << " " << locationVec[i].getRedirection().path << std::endl;
			if (locationVec[i].getAutoIndex() == ON)
				std::cout << "autoindex: on" << std::endl;
		}
	}
}

bool    Parser::findSemicolon() {

	if (line.find(";")  != std::string::npos)
		return (true);
	return (false);
}

void    Parser::printError(int which) {

	status = ERROR;
	switch(which) {

		case SEMICOLON:
			throw excp("Syntax Error: could not find semicolon");
			break ;
		case CURLYBRACE:
			throw excp("Syntax Error: Curly brace not found, or misalighned");
			break ;
		case UNKNOWN:{
			std::string msg = "Error: " + directive + " <- Unknown expression.";
			throw excp(msg);
			break ;
		}
		case EMPTY:{
			std::string msg = "Error: " + directive + " <- Directive can't have empty value!";
			throw excp(msg);
			break ;
		}
		case EXTENSION:
			throw excp("Error: Invalid extension!");
			break ;
		case NO_CONFIG_FILE:
			throw excp("Error: config file not found or invalid!");
			break ;
		case INVALID_STATUS_CODE:{
			std::string msg = "Syntax Error: Invalid status code!\n-> " + line;
			throw excp(msg);
			break ;
		}
		case INVALID_PATH:{
			std::string msg = "Syntax Error: Invalid path!\n-> " + line;
			throw excp(msg);
			break ;
		}
		case FILE_EMPTY:
			throw excp("Error: config file is empty!");
			break ;
	}
}

std::vector<Context> Parser::getParsedData( void ) const
{
	return (parsedData);
}