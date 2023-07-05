/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 20:17:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/05 15:28:01 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configData.hpp"

Location::Location(): autoindex(OFF) {};

Location::Location(std::string value, std::string root, std::string index): value(value), root(root), index(index) {
	autoindex = OFF;
};

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

ServerSettings::ServerSettings() {};

ServerSettings::ServerSettings(std::string port, std::string server_name, Location &location): port(port), server_name(server_name) {

    locations.push_back(location);
};

void    ServerSettings::setPort(const std::string& p) {

    port = p;
}

void    ServerSettings::setName(const std::string& name) {

    server_name = name;
}

void    ServerSettings::setLocations(const Location &location) {

    locations.push_back(location);
}

std::string ServerSettings::getPort() const {

    return (port);
}

std::string ServerSettings::getName() const {

    return (server_name);
}

std::vector<Location>   ServerSettings::getLocations() {

    return (locations);
}

ServerSettings&	ServerSettings::operator=(const ServerSettings& server) {

	if (this != &server)
	{
		this->port = server.port;
		this->server_name = server.server_name;
		this->locations = server.locations;
	}
	return (*this);
}

ServerSettings::~ServerSettings() {};

Context::Context(): status(OK) {};

Context::Context(ServerSettings &server): server(server), status(OK) {};


Context&	Context::operator=(const Context& context) {

	if (this != &context)
		this->server = context.server;
	return (*this);
}

void    Context::setServer(const ServerSettings& servr) {

    server = servr;
}

ServerSettings  Context::getServer() const {

    return (server);
}

void	Context::setStatus(int which) {

	status = which;
}

int	 Context::getStatus() {

	return(status);
}

Context::~Context() {};
