/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 14:10:31 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/06 14:51:28 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <errno.h>
#define SOCKET int

int main( int ac, char **av )
{
    if ( ac < 3 )
    {
        std::cerr << "not enough arguments\n";
        return (EXIT_FAILURE);
    }
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    //TCP connection
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if ( getaddrinfo(av[1], av[2], &hints, &peer_address) != 0 )
    {
        std::cerr << "getaddrinfo failed\n";
        return (EXIT_FAILURE);
    }
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
    if ( socket_peer == -1 )
        std::cerr << "socket is invalid\n";
    if ( connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen) != 0 )
        std::cerr << "connect failed\n";
    freeaddrinfo(peer_address);
    std::cout << "connected!\n";
    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket_peer, &readfds);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000;
        if ( select(socket_peer + 1, &readfds, 0, 0, &timeout) < 0 )
            std::cerr << "select failed\n";
        if ( FD_ISSET( socket_peer, &readfds) )
        {
            char read[4096];
            int bytes_received = recv(socket_peer, read, 4096, 0);
            if ( bytes_received < 1 )
            {
                std::cout << "connection is closed\n";
                break ;
            }
        }
        close(socket_peer);
    }
    return (0);
}