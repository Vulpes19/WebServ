/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/10 14:39:38 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int main( void )
{
    Server srv;
    Client cl;
    fd_set readfds;
    fd_set writefds;

    srv.createListenSocket();
    while ( true )
    {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET( srv.getListenSocket(), &readfds );
        cl.setsManager( srv.getListenSocket(), readfds, writefds );
        if ( FD_ISSET( srv.getListenSocket(), &readfds ) )
        {
            ClientInfo *client = cl.getClient(-1, srv);
            std::cout << "socket connected: " << client->getSocket() << std::endl;
            if ( client->getSocket() == -1 )
            {
                std::cerr << "accept() failed: " << strerror(errno) << std::endl;
                cl.deleteClient(client);
            }
            // printf("New connection from: %s\n", cl.getAddress(client) );
        }
        cl.multiplexing( readfds, writefds );
    }
    close(srv.getListenSocket());
}