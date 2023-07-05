/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 19:42:37 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/05 11:50:41 by abaioumy         ###   ########.fr       */
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

class ServerSettings {
	private:
		std::string             port;
		std::string             server_name;
		std::vector<Location>    locations;
	public:
		ServerSettings();
		ServerSettings(std::string port, std::string server_name, Location &location);
		ServerSettings&	operator=(const ServerSettings& server);
		void    setPort(const std::string &p);
		void    setName(const std::string &name);
		void    setLocations(const Location &location);
		std::string getPort() const;
		std::string getName() const;
		std::vector<Location> getLocations();
		~ServerSettings();
} ;

class Context {
	private:
		ServerSettings  server;
		// Connection		*connection;
		int     status;
	public:
		Context();
		Context(ServerSettings &server);
		Context&    operator=(const Context& context);
		void    setServer(const ServerSettings& servr);
		ServerSettings  getServer() const;
		void    setStatus(int which);
		int	    getStatus();
		~Context();
} ;

#endif