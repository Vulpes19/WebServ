/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 19:42:37 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/19 21:55:27 by abaioumy         ###   ########.fr       */
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
	UNKNOWN,
	EMPTY,
	EXTENSION,
	NO_CONFIG_FILE,
	INVALID_STATUS_CODE,
	INVALID_PATH
} ;

enum {

	ERROR,
	OK
} ;

struct redir {
	redir( void ) : status_code("-1"), path("") {};
	std::string	status_code;
	std::string	path;
};

// class ErrorPage {
// 	private:
// 		int			status_code;
// 		std::string	path;
// 	public:
// 		ErrorPage();
// 		ErrorPage(int status_code, std::string path);
// 		void		setStatusCode(const int code);
// 		void		setPath(const std::string file);
// 		int			getStatusCode() const;
// 		std::string	getPath() const;
// 		~ErrorPage();

// } ;


class Location {
	private:
		std::string value; //darori
		std::string root; //darori
		std::string index; //darori
		std::string	upload;
		redir		redirection;
		int         autoindex ;
		bool		missingValue;
	public:
		Location();
		Location(std::string value, std::string root, std::string index, std::string upload);
		void    setValue(const std::string &val);
		void    setRoot(const std::string &rt);
		void    setIndex(const std::string &indx);
		void    setAutoIndex();
		void	setRedirection(std::string status_code, std::string path);
		void	setUpload(const std::string &upld);
		void	setMissingValue();
		// void	setBraceValue();
		std::string	getUpload() const;
		std::string getValue() const;
		std::string getRoot() const;
		std::string getIndex() const;
		int         getAutoIndex() const;
		bool		getMissingValue() const;
		redir		getRedirection() const;
		Location&	operator=(const Location& location);
		~Location();
} ;

class ServerSettings {
	private:
		std::string             port;  //darori
		std::string				host;
		std::string             server_name; //darori
		std::string				upload;
		ssize_t					body_size;
		std::vector<Location>    locations; //darori
		std::map<std::string, std::string>	errorPages; 
		bool					missingValue;
	public:
		ServerSettings();
		ServerSettings(std::string port, std::string host, std::string server_name, Location &location, int body_size);
		ServerSettings&	operator=(const ServerSettings& server);
		void    setPort(const std::string &p);
		void	initErrorPages();
		void	setSize(const ssize_t &bytes);
		void    setName(const std::string &name);
		void	setMissingValue();
		void	setUpload(const std::string &Upload);
		void    setLocations(const Location &location);
		void	setErrorPages(const std::string& status_code, const std::string& path);
		void	setHost(const std::string &Host);
		std::string	getHost() const;
		std::string getPort() const;
		ssize_t			getSize() const;
		std::string getName() const;
		std::string	getUpload() const;
		bool		getMissingValue() const;
		std::vector<Location> getLocations();
		std::map<std::string, std::string>	getErrorPages();

		~ServerSettings();
} ;

class Context {
	private:
		ServerSettings  server;
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