/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/08 12:16:31 by abaioumy         ###   ########.fr       */
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
        cl.waitClient( srv.getListenSocket(), readfds );
        if ( FD_ISSET( srv.getListenSocket(), &readfds ) )
        {
            struct ClientInfo *client = cl.getClient(-1, srv);
            if ( client->socket == -1 )
            {
                std::cerr << "accept() failed: " << strerror(errno) << std::endl;
                cl.deleteClient(client);
            }
            printf("New connection from: %s\n", cl.getAddress(client) );
        }
        cl.checkClients( readfds );
    }
    close(srv.getListenSocket());
}