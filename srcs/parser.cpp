/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:15 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/12 20:33:42 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parser.hpp"

int main(int argc, char **argv) {

    std::ifstream   confFile(argv[1]);
    if (confFile.is_open())
    {
        std::string line;
        while (getline(confFile, line)) {
            if (line[0] == '#' || line.empty())
                continue ;
            std::stringstream ss(line);
            std::string directive;
            std::string value;
            std::getline(ss, directive, ' ');
            std::cout << "directive: " << directive << std::endl;
            std::cout << "value: " << value << std::endl;
        }
    }
    return 0;
}