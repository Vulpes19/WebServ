/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/20 16:26:21 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parser.hpp"
#include "include/configData.hpp"

Parser::Parser() {};

Parser::~Parser() {};

void    Parser::openFile(char *argv) {

    if (!argv)
        std::cout << "ERROR: File doesn't exist or could not be open!" << std::endl;
    else
    {
        std::ifstream   confFile(argv);
        readFile(confFile);
    }
} ;

void   Parser::readFile(std::ifstream& confFile) {

    if (confFile.is_open())
    {
        while (getline(confFile, line))
        {
            std::stringstream ss(line);
            ss >> directive >> value;
            if (directive == "server" && value[0] == '{')
                parseServer(confFile);
        }
    }
} ;

void    Parser::parseServer(std::ifstream& confFile) {

    Server  server;
    Context context;

    while (getline(confFile, line)) {
        if (line[0] == '#' || line.empty())
            continue ;
        std::stringstream ss(line);
        ss >> directive >> value;
        if (directive == "listen")
            server.setPort(value.erase(value.size() - 1));
        else if (directive == "server_name")
            server.setName(value.erase(value.size() - 1));
        else if (directive == "location")
            parseLocation(confFile, server, value);                
        if (line[0] == '}') {

            context.setServer(server);
            parsedData.push_back(context);
            break ;
        }
    }
}

void    Parser::parseLocation(std::ifstream& confFile, Server& server, std::string& value) {

    Location    location;
    location.setValue(value);
    while(getline(confFile, line)) {
        std::stringstream ss(line);
        ss >> directive >> value;
        if (directive == "}") {
            server.setLocations(location);
            break ;
        }
        if (directive == "root")
            location.setRoot(value.erase(value.size() - 1));
        else if (directive == "index")
            location.setIndex(value.erase(value.size() - 1));
        else if (directive == "autoindex" && value == "on;")
            location.setAutoIndex();
    }
}

void    Parser::printData() {

    for (int i = 0; i < parsedData.size(); i++) {

        Context context;
        context = parsedData[i];

        Server  server;
        server = context.getServer();

        std::cout << std::endl;
        std::cout << "Server: " << std::endl;
        std::cout << "listen: " << server.getPort() << std::endl;
        std::cout << "name: " << server.getName() << std::endl;

        std::vector<Location>   locationVec = server.getLocations();
        for (int i = 0; i < locationVec.size(); i++) {

            std::cout << "locations: " << std::endl;
            std::cout << "value: " << locationVec[i].getValue() << std::endl;
            std::cout << "root: " << locationVec[i].getRoot() << std::endl;
            std::cout << "index: " << locationVec[i].getIndex() << std::endl;
        }
    }
}