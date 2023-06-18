/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:21:33 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 12:05:44 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Libraries.hpp"

class Server
{
    public:
        Server( void );
        ~Server( void );
        void  createListenSocket( void );
        SOCKET  getListenSocket( void ) const;
    private:
        struct addrinfo *bindAddress;
        SOCKET listenSocket;
};