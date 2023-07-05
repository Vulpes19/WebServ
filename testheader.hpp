#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

enum {
	OFF,
	ON
} ;

enum {
	SEMICOLON,
	CURLYBRACE,
	UNKNOWN
} ;

enum {

	ERROR,
	OK
} ;

class Location {
	private:
		std::string value;
		std::string root;
		std::string index;
		int         autoindex;
	public:
		Location();
		Location(std::string value, std::string root, std::string index);
		void    setValue(const std::string &val);
		void    setRoot(const std::string &rt);
		void    setIndex(const std::string &indx);
		void    setAutoIndex();
		std::string getValue() const;
		std::string getRoot() const;
		std::string getIndex() const;
		int         getAutoIndex() const;
		Location&	operator=(const Location& location);
		~Location();
} ;

class Server {
	private:
		std::string             port;
		std::string             server_name;
		std::vector<Location>    locations;
	public:
		Server();
		Server(std::string port, std::string server_name, Location &location);
		Server&	operator=(const Server& server);
		void    setPort(const std::string &p);
		void    setName(const std::string &name);
		void    setLocations(const Location &location);
		std::string getPort() const;
		std::string getName() const;
		std::vector<Location> getLocations();
		~Server();
} ;

class Context {
	private:
		Server  server;
		int     status;
	public:
		Context();
		Context(Server &server);
		Context&    operator=(const Context& context);
		void    setServer(const Server& servr);
		Server  getServer() const;
		void    setStatus(int which);
		int	    getStatus();
		~Context();
} ;

enum {

	PORT,
	NAME,
	ROOT,
	INDEX,
	AUTOINDEX
} ;

class Parser {
	private:
		std::string directive;
		std::string value;
		std::string line;
		std::vector<Context> parsedData;
		int 	openingBraceCount;
		bool    closingBraceExpected;
		int     status;
	public:
		Parser();
		~Parser();
		void    openFile(char *argv);
		void    readFile(std::ifstream&   confFile);
		void    parseServer(std::ifstream&   confFile);
		void    parseLocation(std::ifstream&   confFile, Server& server, std::string& value);
		std::vector<Context> getParsedData();
		void    printData();
		bool    findSemicolon(std::string value);
		void    printError(int which);
		bool    checkBracesError();
		void	setServerContent(Server &server, int which, std::string value);
		void	setLocationContent(Location &location, int which, std::string value);
} ;
