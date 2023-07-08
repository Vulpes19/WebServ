/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libraries.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:04:22 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/05 15:21:13 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <list>
#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <dirent.h>
#include <map>
#include <vector>
#include <ctime>
#include <exception>
#include "parser.hpp"
#define SOCKET int
#define MAX_REQUEST_SIZE 4000
#define BSIZE 1024

class excp : public std::exception
{
    public:
        excp( const std::string &msg ) : message(msg) {};
        virtual ~excp( void ) throw() {};
        virtual const char *what( void ) const throw() { return (message.c_str()); };
    private:
        std::string message;
};