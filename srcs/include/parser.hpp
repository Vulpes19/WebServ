/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:43:59 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/15 20:49:08 by mbaioumy         ###   ########.fr       */
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
    SERVER,
    LOCATION
} ;

class Parser {
    private:
        std::string directive;
        std::string value;
        std::string line;
        std::vector<Context> parsedData;
    public:
        Parser();
        ~Parser();
        void    openFile(char *argv);
        void    readFile(std::ifstream&   confFile);
        void    parseServer(std::ifstream&   confFile);
        void    parseLocation(std::ifstream&   confFile, Server& server);
        std::vector<Context> getParsedData();
        void    printData();
} ;

#endif