/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 19:42:37 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/15 20:48:18 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGDATA_HPP
#define CONFIGDATA_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

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
    public:
        Context();
        Context(Server &server);
        Context&    operator=(const Context& context);
        void    setServer(const Server& servr);
        Server  getServer() const;
        ~Context();
} ;


#endif