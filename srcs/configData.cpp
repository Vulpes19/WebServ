/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 20:17:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/14 08:04:34 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configData.hpp"

ErrorPage::ErrorPage() {} ;

ErrorPage::ErrorPage(int status_code, std::string path): status_code(status_code), path(path) {} ;

void	ErrorPage::setStatusCode(const int code) {

	status_code = code;
}

void	ErrorPage::setPath(const std::string file) {

	path = file;
} 

int		ErrorPage::getStatusCode() const {

	return (status_code);
}

std::string	ErrorPage::getPath() const {

	return (path);
}

ErrorPage::~ErrorPage() {} ;


Location::Location(): upload("default"), autoindex(OFF), missingValue(true) {};

Location::Location(std::string value, std::string root, std::string index, std::string upload): value(value), root(root), index(index), upload(upload) {
	autoindex = OFF;
};

void	Location::setMissingValue() {

	missingValue = false;
}


void    Location::setValue(const std::string& val) {

    value = val;
}

void	Location::setUpload(const std::string &upld) {
	
	upload = upld;	
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


std::string Location::getUpload() const {

	return (upload);
}

std::string Location::getValue() const {

    return (value);
}

bool	Location::getMissingValue() const {
	return (missingValue);
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

ServerSettings::ServerSettings(): host("localhost"), body_size(10000), missingValue(true) {};

ServerSettings::ServerSettings(std::string port, std::string host, std::string server_name, Location &location, int body_size): port(port), host(host), server_name(server_name), body_size(body_size) {

    locations.push_back(location);
};

void    ServerSettings::setPort(const std::string& p) {

    port = p;
}

void	ServerSettings::setMissingValue() {

	missingValue = false;
}

void	ServerSettings::setHost(const std::string &Host) {

	host = Host;
}

void    ServerSettings::setName(const std::string& name) {

    server_name = name;
}

void	ServerSettings::setSize(const int& bytes) {

	body_size = bytes;
}

void    ServerSettings::setLocations(const Location &location) {
    locations.push_back(location);
}

void	ServerSettings::setErrorPages(const ErrorPage& error_page) {

	errorPages.push_back(error_page);
};

std::string ServerSettings::getPort() const {
    return (port);
}

std::string	ServerSettings::getHost() const {

	return (host);
}

bool	ServerSettings::getMissingValue() const {
	return (missingValue);
}

int	ServerSettings::getSize() const {

	return (body_size);
}

std::string ServerSettings::getName() const {
    return (server_name);
}

std::vector<Location>   ServerSettings::getLocations() {

    return (locations);
}

std::vector<ErrorPage>	ServerSettings::getErrorPages() {

	return (errorPages);
}

ServerSettings&	ServerSettings::operator=(const ServerSettings& server) {
	if (this != &server)
	{
		this->port = server.port;
		this->server_name = server.server_name;
		this->locations = server.locations;
		this->errorPages = server.errorPages;
		this->body_size = server.body_size;
		this->host = server.host;
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