/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:43:59 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 18:06:28 by mbaioumy         ###   ########.fr       */
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
    PORT,
    NAME,
    LOCATION,
    ROOT,
    INDEX,
    ALIAS
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
        Location&	operator=(const Location& location);
        ~Location();
} ;

class Server {
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
        ~Context();
} ;
#endif