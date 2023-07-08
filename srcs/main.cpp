/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/08 20:03:54 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Connection.hpp"
#include "Libraries.hpp"

void	initServers( std::vector<Server> &servers, Parser &parser )
{
	std::vector<Context> contexts = parser.getParsedData();
	for ( size_t i = 0; i < contexts.size(); i++ )
	{
		ServerSettings settings;
		Server server;
		settings = contexts[i].getServer();
		server.setName(settings.getName());
		server.setPort(settings.getPort());
		server.setLocations(settings.getLocations());
		// servers[settings.getName()] = server;
		servers.push_back(server);
	}
	// std::map<std::string, Server>::iterator it = servers.begin();
	// for ( ; it != servers.end(); ++it)
	// 	it->second.createListenSocket();
	for ( size_t i = 0; i < servers.size(); i++)
		servers[i].createListenSocket();
}

int main( int ac, char **av )
{
	std::vector<Server> servers;
	Connection cl;
	Parser	parser;
	fd_set readfds;
	fd_set writefds;

	try
	{
		if ( ac != 2 )
			return (EXIT_FAILURE);
		parser.openFile(av[1]);
		initServers( servers, parser );
		while ( true )
		{
			cl.setsManager( servers, readfds, writefds );
			// std::map<std::string, Server>::iterator it = servers.begin();
			for ( size_t i = 0; i < servers.size(); i++ )
			{
				if ( FD_ISSET( servers[i].getListenSocket(), &readfds ) )
				{
					ClientManager *client = cl.getClient(-1, servers[i]);
					if ( client->getSocket() == -1 )
						cl.deleteClient(client);
				}
			}
			cl.multiplexing( readfds, writefds );
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	// close(srv.getListenSocket());
	return (0);
}

// int main( int ac, char **av )
// {
// 	std::vector<Server> servers;
// 	// Connection cl;
// 	Parser	parser;
// 	fd_set readfds;
// 	fd_set writefds;

// 	try
// 	{
// 		if ( ac != 2 )
// 			return (EXIT_FAILURE);
// 		parser.openFile(av[1]);
// 		initServers( servers, parser );
// 		while ( true )
// 		{
// 			for ( size_t i = 0; i < servers.size(); i++ )
// 			{
// 				servers[i].setsManager( readfds, writefds );
// 				if ( FD_ISSET( servers[i].getListenSocket(), &readfds ) )
// 				{
// 					ClientManager *client = servers[i].getClient(-1);
// 					std::cout << "socket connected: " << client->getSocket() << std::endl;
// 					if ( client->getSocket() == -1 )
// 					{
// 						std::cerr << "accept() failed: " << strerror(errno) << std::endl;
// 						servers[i].deleteClient(client);
// 					}
// 				}
// 				servers[i].multiplexing( readfds, writefds );
// 			}
// 		}
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	// close(srv.getListenSocket());
// 	return (0);
// }