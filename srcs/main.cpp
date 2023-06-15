/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/13 09:24:58 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Connection.hpp"

int main( void )
{
    Server srv;
    Connection cl;
    fd_set readfds;
    fd_set writefds;

    srv.createListenSocket();
    while ( true )
    {
        cl.setsManager( srv.getListenSocket(), readfds, writefds );
        if ( FD_ISSET( srv.getListenSocket(), &readfds ) )
        {
            ClientManager *client = cl.getClient(-1, srv);
            std::cout << "socket connected: " << client->getSocket() << std::endl;
            if ( client->getSocket() == -1 )
            {
                std::cerr << "accept() failed: " << strerror(errno) << std::endl;
                cl.deleteClient(client);
            }
        }
        cl.multiplexing( readfds, writefds );
    }
    system("leaks webserv");
    close(srv.getListenSocket());
}