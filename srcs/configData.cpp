/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 20:17:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/20 16:30:34 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/configData.hpp"

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

Server  Context::getServer() const {

    return (server);
}

Context::~Context() {};
