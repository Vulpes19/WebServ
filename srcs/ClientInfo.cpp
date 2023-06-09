/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:03:47 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 12:36:15 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientInfo.hpp"

ClientInfo::ClientInfo( void )
{}

ClientInfo::~ClientInfo( void )
{
	close(socket);
}

// void    ClientInfo::reset( void )
// {
// 	memset(&address, 0, sizeof(address));
// 	memset(request, 0, sizeof(request));
// 	addressLen = 0;
// 	socket = -1;
// 	bytesReceived = 0;
// 	isReading = false;
// 	isWriting = false;
// }

bool    ClientInfo::handleReadRequest( void )
{
	
}

void    ClientInfo::handleProcessRequest( void )
{}

void    ClientInfo::handleWriteRequest( void )
{}

void    ClientInfo::changeSet( fd_set &from, fd_set &to )
{}