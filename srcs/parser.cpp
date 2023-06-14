/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 18:06:08 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parser.hpp"

Location::Location() {};

Location::Location(std::string value, std::string root, std::string index): value(value), root(root), index(index) {};

void    Location::setValue(const std::string& val) {

    value = val;
}

void    Location::setRoot(const std::string& rt) {

    root = rt;
}

void    Location::setIndex(const std::string& indx) {

    index = indx;
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

    locations.push(location);
};

void    Server::setPort(const std::string& p) {

    port = p;
}

void    Server::setName(const std::string& name) {

    server_name = name;
}

void    Server::setLocations(const Location &location) {

    locations.push(location);
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

Context::Context() {};

Context::Context(Server &server): server(server) {};


Context&	Context::operator=(const Context& context) {

	if (this != &context)
		this->server = context.server;
	return (*this);
}

void    Context::setServer(const Server& servr) {

    server = servr;
}

Context::~Context() {};

int main(int argc, char **argv) {

    std::ifstream       confFile(argv[1]);
    std::stack<Context> nestedDirs;
    if (confFile.is_open())
    {
        std::string line;
        while (getline(confFile, line))
        {    
            std::stringstream ss(line);
            std::string directive, value;
            ss >> directive >> value;
            if (directive == "server" && value[0] == '{')
            {
                Server  server;
                Context context;
                while (getline(confFile, line)) {
                    if (line[0] == '#' || line.empty())
                        continue ;
                    std::stringstream ss(line);
                    std::string directive, value;
                    ss >> directive >> value;
                    if (directive == "listen")
                        server.setPort(value);
                    else if (directive == "server_name")
                        server.setName(value);
                    else if (directive == "location") {
                            
                            Location    location;
                            location.setValue(value);
                            while(getline(confFile, line)) {
                                
                                std::stringstream ss(line);
                                std::string directive, value;
                                ss >> directive >> value;
                                if (directive == "root")
                                    location.setRoot(value);
                                else if (directive == "index")
                                    location.setIndex(value);
                                if (line[0] == '}') {
                                    
                                    server.setLocations(location);
                                    break ;
                                }
                            }
                    }                       
                    if (line[0] == '}') {

                        context.setServer(server);
                        nestedDirs.push(context);
                        break ;
                    }
                }
            }
        }
    }
    
    return 0;
}