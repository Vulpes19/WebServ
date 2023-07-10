/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 15:32:05 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/10 13:50:12 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Resources.hpp"
#include "/Users/mbaioumy/Documents/WebServ/include/configData.hpp"
#include "/Users/mbaioumy/Documents/WebServ/include/parser.hpp"

int main()
{
    // Resources   resources;

    // resources.checkRequest("GET /api/user HTTP/1.1\r\nHost: website.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\r\nContent-Type: application/json\r\nContent-Length: 81\r\n\r\n{\"username\": \"<script>alert('XSS attack!');</script>\", \"password\": \"secretpass\"}");
    // return (0);

    Parser parser;
    parser.openFile(argv[1]);
    parser.printData();
}