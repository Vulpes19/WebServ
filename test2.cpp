#include "testheader.hpp"

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

		std::vector<Location>   locationVec = server.getLocations();
		for (int i = 0; i < locationVec.size(); i++) {

			std::cout << "locations: " << std::endl;
			std::cout << "value: " << locationVec[i].getValue() << std::endl;
			std::cout << "root: " << locationVec[i].getRoot() << std::endl;
			std::cout << "index: " << locationVec[i].getIndex() << std::endl;
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


Location::Location(): autoindex(OFF) {};

Location::Location(std::string value, std::string root, std::string index): autoindex(OFF), value(value), root(root), index(index) {};

void    Location::setValue(const std::string& val) {

    value = val;
}

void    Location::setRoot(const std::string& rt) {

    root = rt;
}

void    Location::setIndex(const std::string& indx) {

    index = indx;
}

void	Location::setAutoIndex() {
	
	autoindex = ON;
}

std::string Location::getValue() const {

    return (value);
}

std::string Location::getRoot() const {

    return (root);
}

std::string Location::getIndex() const {

    return(index);
}

int			Location::getAutoIndex() const {

	return(autoindex);
}

Location&	Location::operator=(const Location& location) {

	if (this != &location)
	{
		this->value = location.value;
		this->root = location.root;
		this->index = location.index;
	}
	return (*this);
}

Location::~Location() {};

Server::Server() {};

Server::Server(std::string port, std::string server_name, Location &location): port(port), server_name(server_name) {

    locations.push_back(location);
};

void    Server::setPort(const std::string& p) {

    port = p;
}

void    Server::setName(const std::string& name) {

    server_name = name;
}

void    Server::setLocations(const Location &location) {

    locations.push_back(location);
}

std::string Server::getPort() const {

    return (port);
}

std::string Server::getName() const {

    return (server_name);
}

std::vector<Location>   Server::getLocations() {

    return (locations);
}

Server&	Server::operator=(const Server& server) {

	if (this != &server)
	{
		this->port = server.port;
		this->server_name = server.server_name;
		this->locations = server.locations;
	}
	return (*this);
}

Server::~Server() {};

Context::Context(): status(OK) {};

Context::Context(Server &server): server(server), status(OK) {};


Context&	Context::operator=(const Context& context) {

	if (this != &context)
		this->server = context.server;
	return (*this);
}

void    Context::setServer(const Server& servr) {

    server = servr;
}

Server  Context::getServer() const {

    return (server);
}

void	Context::setStatus(int which) {

	status = which;
}

int	 Context::getStatus() {

	return(status);
}

Context::~Context() {};
