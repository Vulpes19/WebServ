/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 20:17:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/18 17:56:05 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configData.hpp"

// ErrorPage::ErrorPage() { } ;

// ErrorPage::ErrorPage(int status_code, std::string path): status_code(status_code), path(path) {} ;

// void	ErrorPage::setStatusCode(const int code) {

// 	status_code = code;
// }

// void	ErrorPage::setPath(const std::string file) {

// 	path = file;
// } 

// int		ErrorPage::getStatusCode() const {

// 	return (status_code);
// }

// std::string	ErrorPage::getPath() const {

// 	return (path);
// }

// ErrorPage::~ErrorPage() {} ;


Location::Location(): upload("default") ,autoindex(OFF), missingValue(true) {

	redirection.status_code = "-1"; 
	redirection.path = ""; 
};

Location::Location(std::string value, std::string root, std::string index, std::string upload): value(value), root(root), index(index), upload(upload) {
	autoindex = OFF;
	redirection.status_code = "-1";
	redirection.path = "";
};

void	Location::setMissingValue() {

	missingValue = false;
}

void	Location::setRedirection(std::string status_code, std::string path) {

	redirection.status_code = status_code;
	redirection.path = path;
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

redir		Location::getRedirection() const {

	return (redirection);
}

Location&	Location::operator=(const Location& location) {

	if (this != &location)
	{
		this->value = location.value;
		this->root = location.root;
		this->index = location.index;
		this->redirection = location.redirection;
	}
	return (*this);
}

Location::~Location() {};

ServerSettings::ServerSettings(): host("localhost"), body_size(10000000), missingValue(true) {};

ServerSettings::ServerSettings(std::string port, std::string host, std::string server_name, Location &location, int body_size): port(port), host(host), server_name(server_name), body_size(body_size) {

    locations.push_back(location);
};

void    ServerSettings::setPort(const std::string& p) {

    port = p;
}

void	ServerSettings::initErrorPages() {
	
	errorPages["400"] = "assets/error_images/400.html";
	errorPages["401"] = "assets/error_images/401.html";
	errorPages["403"] = "assets/error_images/403.html";
	errorPages["404"] = "assets/error_images/404.html";
	errorPages["405"] = "assets/error_images/405.html";
	errorPages["411"] = "assets/error_images/411.html";
	errorPages["413"] = "assets/error_images/413.html";
	errorPages["500"] = "assets/error_images/500.html";
	errorPages["505"] = "assets/error_images/505.html";
}

void	ServerSettings::setMissingValue() {

	missingValue = false;
}

void	ServerSettings::setUpload(const std::string &Upload) {

	upload = Upload;
}

void	ServerSettings::setHost(const std::string &Host) {

	host = Host;
}

void    ServerSettings::setName(const std::string& name) {

    server_name = name;
}

void	ServerSettings::setSize(const ssize_t& bytes) {

	if (bytes > 10000000)
		body_size = bytes;
}

void    ServerSettings::setLocations(const Location &location) {
    locations.push_back(location);
}

void	ServerSettings::setErrorPages(const std::string& status_code, const std::string& path) {

	errorPages[status_code] = path;
};

std::string ServerSettings::getUpload() const {

	return (upload);
}

std::string ServerSettings::getPort() const {
    return (port);
}

std::string	ServerSettings::getHost() const {

	return (host);
}

bool	ServerSettings::getMissingValue() const {
	return (missingValue);
}

ssize_t	ServerSettings::getSize() const {

	return (body_size);
}

std::string ServerSettings::getName() const {
    return (server_name);
}

std::vector<Location>   ServerSettings::getLocations() {

    return (locations);
}

std::map<std::string, std::string>	ServerSettings::getErrorPages() {

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
		this->upload = server.upload;
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