/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libraries.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:04:22 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/04 10:21:32 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include <ctime>
#define SOCKET int
#define MAX_REQUEST_SIZE 4000

#define BSIZE 1024
