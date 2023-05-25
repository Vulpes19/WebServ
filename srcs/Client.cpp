/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/25 13:31:07 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client( void )
{}

Client::~Client( void )
{}

struct ClientInfo   *Client::getClient( SOCKET socket )
{
    iterator it = clients.find(socket);
    if ( it != clients.end() )
        return (it->second);
    struct ClientInfo *newClient = new ClientInfo();
    newClient->addressLen = sizeof(newClient->address);
    clients.insert(pair(socket, newClient));
    return (newClient);
}

void    Client::deleteClient( struct ClientInfo *cl )
{
    iterator it = clients.find(cl->socket);
    if ( it != clients.end() )
        clients.erase(it);
    std::cerr << "dropped client not found\n";
}

const char  *Client::getAddress( SOCKET socket )
{
    
}

fd_set  Client::waitClient( SOCKET socket )
{
    fd_set 
}