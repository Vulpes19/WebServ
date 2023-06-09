/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   States.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:35:40 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 12:36:24 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "States.hpp"

void    States::reset( ClientInfo *client )
{
    memset(&address, 0, sizeof(address));
	memset(request, 0, sizeof(request));
	addressLen = 0;
	socket = -1;
	bytesReceived = 0;
	isReading = false;
	isWriting = false;
    path = NULL;
}