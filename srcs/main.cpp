/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 11:19:23 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/21 15:51:14 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <time.h>
#include <cstdio>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#define SOCKET int
 
int main( void )
{
	struct addrinfo hints;
	struct addrinfo *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if ( getaddrinfo( 0, "8080", &hints, &res ) != 0 )
		std::cerr << "getaddrinfo failed\n";
	SOCKET socket_listen;
	socket_listen = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if ( socket_listen == -1 )
		std::cerr << "socket faield\n";
	if ( bind( socket_listen, res->ai_addr, res->ai_addrlen ) )
		std::cerr << "bind failed\n";
	freeaddrinfo(res);
	std::cout << "listening...\n";
	if ( listen(socket_listen, 10) == -1 )
		std::cerr << "listen failed\n";
	std::cout << "waiting for connection...\n";
	struct sockaddr_storage addr;
	socklen_t sock_len = sizeof(addr);
	SOCKET socket_client = accept(socket_listen, (struct sockaddr *) &addr, &sock_len);
	if ( socket_client == -1 )
		std::cerr << "accept failed\n";
	std::cout << "client is connected\n";
	char request[1024];
	int bytes_received = recv( socket_client, request, 1024, 0 );
	(void)bytes_received;
	const char *response = 
		"HTTP/1.1 200 OK\r\n"
		"Connection: close\r\n"
		"Content-Type: text/plain\r\n\r\n"
		"Local time is: ";
	int bytes_sent = send(socket_client, response, strlen(response), 0);
	if ( bytes_sent == -1 )
		std::cerr << "send failed\n";
	time_t timer;
	time(&timer);
	char *time_msg = ctime(&timer);
	bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
	if ( bytes_sent == -1 )
		std::cerr << "send failed\n";
	close(socket_client);
	close(socket_listen);
	return (0);
}