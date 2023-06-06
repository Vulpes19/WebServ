/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/06 16:46:41 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include "Client.hpp"

int main( void )
{
    Server srv;
    Client cl;

    srv.createListenSocket();
    while ( true )
    {
        fd_set readfds;
        readfds = cl.waitClient(srv.getListenSocket());
        if ( FD_ISSET(srv.getListenSocket(), &readfds) )
        {
            struct ClientInfo *client = cl.getClient(-1, srv);
            if ( client->socket == -1 )
            {
                std::cerr << "accept() failed: " << strerror(errno) << std::endl;
                return (EXIT_FAILURE);
            }
            printf("New connection from: %s\n", cl.getAddress(client) );
        }
        cl.checkClients(readfds);
    }
    close(srv.getListenSocket());
}