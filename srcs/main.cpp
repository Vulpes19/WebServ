<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 18:42:12 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/15 20:49:18 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/parser.hpp"

int main(int argc, char **argv) {

    Parser  parser;

    parser.openFile(argv[1]);
    parser.printData();
    return 0;
=======

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
>>>>>>> 7605baa527585389566f2b8bc3bb1441820d455c
}
