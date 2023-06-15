/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:43:59 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/15 19:44:27 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <sstream>
#include <algorithm>
#include "configData.hpp"

enum {
    SERVER,
    LOCATION
} ;

class Parser {
    private:
        std::stringstream ss;
        std::string directive;
        std::string value;
        std::string line;
        std::ifstream   confFile;
    public:
        void    openFile(char *argv);
        void    parseServer();
        void    parseLocation();
        std::stack<Context> parsedData();
} ;

#endif