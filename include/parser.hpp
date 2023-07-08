/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:43:59 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/08 18:24:17 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include "configData.hpp"


enum {

	PORT,
	NAME,
	ROOT,
	INDEX,
	AUTOINDEX,
	SIZE,
	UPLOAD,
	ERROR_PAGE,
	RETURN,
	HOST
} ;

class Parser {
	private:
		std::string directive;
		std::string value;
		std::string line;
		std::vector<Context> parsedData;
		int 	openingBraceCount;
		bool	host_exists;
		bool    closingBraceExpected;
		int     status; 
	public:
		Parser();
		~Parser();
		void    openFile(char *argv);
		void    readFile(std::ifstream&   confFile);
		void    parseServer(std::ifstream&   confFile);
		void    parseLocation(std::ifstream&   confFile, ServerSettings& server, std::string& value);
		void    printData();
		bool    findSemicolon(std::string value);
		void    printError(int which);
		bool    checkBracesError();
		void	setServerContent(ServerSettings &server, int which, std::string value);
		void	setLocationContent(Location &location, int which, std::string value);
		std::vector<Context> getParsedData() const;
} ;

#endif