/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:43:59 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 15:14:40 by mbaioumy         ###   ########.fr       */
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


class Location {
    private:
        std::string value;
        std::string root;
        std::string index;
    public:
        Location(std::string value, std::string root, std::string index);
        ~Location();
}

class Server {
    private:
        std::string             port;
        std::string             server_name;
        std::stack<Location>    locations;
    public:
        Server(std::string port, std::string server_name);
        ~Server();
}

class Context {
    private:
        Server  server;
    public:
        Context(Server server);
        ~Context();
} ;
#endif