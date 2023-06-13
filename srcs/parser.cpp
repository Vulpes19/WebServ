/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/13 21:22:34 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parser.hpp"

Context::Context(std::string& type): type(type) {};

Context::~Context() {};

int main(int argc, char **argv) {

    std::ifstream       confFile(argv[1]);
    std::stack<Context> nestedDirs;
    Context cont;
    if (confFile.is_open())
    {
        std::string line;
        while (getline(confFile, line)) {
            if (line[0] == '#' || line.empty())
                continue ;
            std::stringstream ss(line);
            std::string directive, value;
            ss >> directive >> value;
            if (value[0] == '{')
            {
                cont.type = directive;
                cont.block[]
            }
        }
    }
    return 0;
}