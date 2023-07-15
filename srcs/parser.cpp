/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/15 14:42:00 by abaioumy         ###   ########.fr       */
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

int		countWords(std::string str) {

	std::stringstream ss(str);
	int	words = 0;
	std::string	word;

	while (ss >> word)
		words++;
	return (words);
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
			std::stringstream ss(value);
			std::string path;
			std::string	str;
			int			words = countWords(value);
			std::string	status_code;

			if (words == 2) {
				ss >> str >> status_code;
				if (status_code.size() - 1 > 0) {
					if (findSemicolon(status_code))
						location.setRedirection(status_code.erase(status_code.size() - 1), "");
					else
						printError(SEMICOLON);				
				}
				else
					printError(EMPTY);
			}
			else if (words == 3) {	
				ss >> str >> status_code >> path;
				if (path.size() - 1 > 0) {
					if (findSemicolon(path))
						location.setRedirection(status_code, path.erase(path.size() - 1));
					else
						printError(SEMICOLON);				
				}
				else
					printError(EMPTY);
			}
			else if (words > 3) {
				std::cerr << "Syntax Error: Return field is invalid, too many elements!" << std::endl;
				exit(1);
			}
			else {
				printError(EMPTY);
				break ;
			}
			break ;
	}
}

void   Parser::readFile(std::ifstream& confFile) {

	// t_brace	brace;

	// brace.openingBrace = false;
	// brace.closingBrace = false;
	if (confFile.is_open())
	{
		while (getline(confFile, line))
		{
			if (line[0] == '#' || line.empty())
				continue ;
			if (line.find("server") != std::string::npos)
			{
				// std::cout << "line: " << line << std::endl;
				// std::string	nextLine;

				// if (line.find("{") != std::string::npos) {
				// 	brace.openingBrace = true;
					parseServer(confFile);
				// }
				// else {
				// 	while (getline(confFile, nextLine)) {
				// 		if (nextLine.size())
				// 		{
				// 			if (nextLine.find("{") != std::string::npos && nextLine.find("location") == std::string::npos) {
				// 				brace.openingBrace = true;
				// 				parseServer(confFile);
				// 			}
				// 		}
				// 	}
				// }
			}
			// }
			// 	else {
			// 		while (getline(confFile, nextLine)) {
			// 			if (nextLine.size())
			// 			{
			// 				if (nextLine.find("{") != std::string::npos && nextLine.find("location") == std::string::npos) {
			// 					brace.openingBrace = true;
			// 					parseServer(confFile);
			// 				}
			// 			}
			// 		}
			// 	}
			// }
			// if (line.find("}") != std::string::npos)
			// {
			// 	brace.closingBrace = true;
			// 	break ;
			// }
		}
		// if (!brace.closingBrace || !brace.openingBrace) {
		// 	std::cerr << "Syntax Error: Braces error!" << std::endl;
		// 	exit(1);
		// }
	}
	else
		std::cout << "Error: could not open the configuration file!" << std::endl;
} ;

void	Parser::serverValuesValidation(ServerSettings server) {

	std::string port = server.getPort();
	int			portInt;
	std::stringstream ss(port);

	ss >> portInt;
	if (port.size() == 0) {
		std::cerr << "Syntax Error: port not found!" << std::endl;
		exit(1);
	}
	if (server.getName().size() == 0) {	
		std::cerr << "Syntax Error: server name not found!" << std::endl;
		exit(1);
	}
	if (portInt == 0) {	
		std::cerr << "Syntax Error: port is invalid!" << std::endl;
		exit(1);
	}
	if (server.getLocations().size() == 0) {
		std::cerr << "Syntax Error: Location is undefined!" << std::endl;
		exit(1);
	}
}

void	checkBraces(bool brace) {
	
	if (!brace) {
		std::cerr << "Syntax Error: Braces error!" << std::endl;
		exit(1);
	}
}

void	Parser::parseServer(std::ifstream& confFile) {

	ServerSettings  server;
	Context context;
	std::string optionalVal;
	t_brace		brace;

	brace.closingBrace = false;
	brace.openingBrace = false;
	// std::cout << "line: " << line << std::endl;
	while (getline(confFile, line)) {

		if (line[0] == '#' || line.empty())
			continue ;
		std::stringstream ss(line);
		ss >> directive >> value >> optionalVal;
		// brace.openingBrace = false;
		// if (line.find("{") != std::string::npos)
		// 	brace.openingBrace = true;
		if (directive.find("listen") != std::string::npos)
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
			// checkBraces(brace.openingBrace);	
			parseLocation(confFile, server, value);              
		}
		else if (line.find("}") != std::string::npos) {
			// brace.closingBrace = true;
			context.setServer(server);
			parsedData.push_back(context);
			break ;
		}
	}
	serverValuesValidation(server);
	// if (!brace.closingBrace || !brace.openingBrace) {
	// 	std::cerr << "Syntax Error: Braces error!" << std::endl;
	// 	exit(1);
	// }
}

bool	examinePath(std::string value) {

	if (value.find("/") != std::string::npos)
		return (true);
	return (false);
}

void	Parser::locationValuesValidation(Location location) {

	if (location.getValue().size() == 0 || !examinePath(location.getValue())) {
		std::cerr << "Syntax Error: location value not found or invalid!" << std::endl;
		exit(1);
	}
	if (location.getRoot().size() == 0 || !examinePath(location.getRoot())) {
		std::cerr << "Syntax Error: location root value not found or invalid!" << std::endl; 
		exit(1);
	}
	if (location.getIndex().size() == 0) {
		location.setIndex("HOMEPAGE");
	}
	if ((location.getRedirection().status_code.size() > 3 || location.getRedirection().status_code.size() < 3) && 
		location.getRedirection().status_code != "-1") {
		std::cerr << "Syntax Error: return status code is invalid!" << std::endl;
		exit(1);
	}
	std::stringstream ss(location.getRedirection().status_code);
	int				  code;
	
	ss >> code;
	if (code == 0) {
		std::cerr << "Syntax Error: return status code is invalid!" << std::endl;
		exit(1);
	}
}

void	Parser::parseLocation(std::ifstream& confFile, ServerSettings& server, std::string& value) {

	Location	location;
	t_brace		brace;

	brace.openingBrace = false;
	brace.closingBrace = false;
	
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
			setLocationContent(location, RETURN, line);
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
			std::cout << "return: " << locationVec[i].getRedirection().status_code; 
			std::cout << " " << locationVec[i].getRedirection().path << std::endl;
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