/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 19:42:37 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/06 14:18:51 by mbaioumy         ###   ########.fr       */
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
	EMPTY
} ;

enum {

	ERROR,
	OK
} ;

class ErrorPage {
	private:
		int			status_code;
		std::string	path;
	public:
		ErrorPage();
		ErrorPage(int status_code, std::string path);
		void		setStatusCode(const int code);
		void		setPath(const std::string file);
		int			getStatusCode() const;
		std::string	getPath() const;
		~ErrorPage();

} ;

class Location {
	private:
		std::string value;
		std::string root;
		std::string index;
		std::string	upload;
		int         autoindex;
	public:
		Location();
		Location(std::string value, std::string root, std::string index, std::string upload);
		void    setValue(const std::string &val);
		void    setRoot(const std::string &rt);
		void    setIndex(const std::string &indx);
		void    setAutoIndex();
		void	setUpload(const std::string &upld);
		std::string	getUpload() const;
		std::string getValue() const;
		std::string getRoot() const;
		std::string getIndex() const;
		int         getAutoIndex() const;
		Location&	operator=(const Location& location);
		~Location();
} ;

class Server {
	private:
		std::string             port;
		std::string             server_name;
		int						body_size;
		std::vector<Location>    locations;
		std::vector<ErrorPage>	errorPages;
	public:
		Server();
		Server(std::string port, std::string server_name, Location &location, int body_size);
		Server&	operator=(const Server& server);
		void    setPort(const std::string &p);
		void	setSize(const int &bytes);
		void    setName(const std::string &name);
		void    setLocations(const Location &location);
		void	setErrorPages(const ErrorPage &error_page);
		std::string getPort() const;
		int			getSize() const;
		std::string getName() const;
		std::vector<Location> getLocations();
		std::vector<ErrorPage>	getErrorPages();
		~Server();
} ;

class Context {
	private:
		Server  server;
		int     status;
	public:
		Context();
		Context(Server &server);
		Context&    operator=(const Context& context);
		void    setServer(const Server& servr);
		Server  getServer() const;
		void    setStatus(int which);
		int	    getStatus();
		~Context();
} ;

#endif