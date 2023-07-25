/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 09:50:28 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/25 10:59:29 by abaioumy         ###   ########.fr       */
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
		server.setHost(settings.getHost());
		server.setPort(settings.getPort());
		server.setLocations(settings.getLocations());
		server.setBodySize(settings.getSize());
		server.setErrorPages(settings.getErrorPages());
		server.setUpload(settings.getUpload());
		servers.push_back(server);
	}
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
	std::string serverName = "NONE";

	try
	{
		if ( ac != 2 )
		{
			std::cerr << "error please enter: ./webserv [config file]\n";
			return (EXIT_FAILURE);
		}
		parser.openFile(av[1]);
		// parser.printData();
		// exit(1);
		initServers( servers, parser );
		while ( true )
		{
			cl.setsManager( servers, readfds, writefds );
			for ( size_t i = 0; i < servers.size(); i++ )
			{
				if ( FD_ISSET( servers[i].getListenSocket(), &readfds ) )
				{
					ClientManager *client = cl.getClient(-1, servers[i]);
					if ( client->getSocket() == -1 )
						cl.deleteClient(client);
				}
			}
			cl.multiplexing( readfds, writefds, serverName, servers );
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	// close(srv.getListenSocket());
	return (0);
}
