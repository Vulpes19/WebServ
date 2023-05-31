/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/31 15:11:32 by abaioumy         ###   ########.fr       */
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
        fd_set reads;
        reads = cl.waitClient(srv.getListenSocket());
        if ( FD_ISSET(srv.getListenSocket(), &reads) )
        {
            struct ClientInfo *client = cl.getClient(-1, srv);
            if ( client->socket == -1 )
            {
                std::cerr << "accept() failed: " << strerror(errno) << std::endl;
                return (EXIT_FAILURE);
            }
            printf("New connection from: %s\n", cl.getAddress(client) );
        }   
            cl.checkClients(reads);
    }
    close(srv.getListenSocket());
}