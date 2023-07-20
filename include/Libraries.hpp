/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libraries.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vulpes <vulpes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:04:22 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/20 13:05:22 by vulpes           ###   ########.fr       */
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

enum    SERVER_SETTINGS_STATUS
{
    UPDATE_CLIENT_SETTINGS,
    KEEP_CLIENT_SETTINGS
};

enum TYPES
{
    TYPE_NAME,
    TYPE_SUFFIX
};

class excp : public std::exception
{
    public:
        excp( const std::string &msg ) : message(msg) {};
        virtual ~excp( void ) throw() {};
        virtual const char *what( void ) const throw() { return (message.c_str()); };
    private:
        std::string message;
};