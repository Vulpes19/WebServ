/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:43:59 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/15 12:01:32 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <sstream>
#include <algorithm>

enum {
    SERVER,
    LOCATION
} ;

class Parser {
    private:
        std::stringstream ss;
        std::string directive;
        std::string value;
        std::string line;
        std::ifstream   confFile;
} ;

class Location {
    private:
        std::string value;
        std::string root;
        std::string index;
    public:
        Location();
        Location(std::string value, std::string root, std::string index);
        void    setValue(const std::string &val);
        void    setRoot(const std::string &rt);
        void    setIndex(const std::string &indx);
        std::string getValue() const;
        std::string getRoot() const;
        std::string getIndex() const;
        Location&	operator=(const Location& location);
        ~Location();
} ;

class ServerParsing {
    private:
        std::string             port;
        std::string             server_name;
        std::stack<Location>    locations;
    public:
        Server();
        Server(std::string port, std::string server_name, Location &location);
        Server&	operator=(const Server& server);
        void    setPort(const std::string &p);
        void    setName(const std::string &name);
        void    setLocations(const Location &location);
        std::string getPort() const;
        std::string getName() const;
        Location getLocations();
        ~Server();
} ;

class Context {
    private:
        Server  server;
    public:
        Context();
        Context(Server &server);
        Context&    operator=(const Context& context);
        void    setServer(const Server& servr);
        Server  getServer() const;
        ~Context();
} ;
#endif