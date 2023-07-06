/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 15:32:05 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/06 15:40:48 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/Users/mbaioumy/Documents/WebServ/include/Resources.hpp"

int main()
{

    Resources   resources;

    resources.checkRequest("GET /XXX HTTP/1.1\r\nHost: website.com\r\nUser-Agent: Chrome/5.0 (Windows 10)\r\nAccept-Language: en-US\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\n");

    return (0);
}