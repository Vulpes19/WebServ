/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testMain.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 15:32:05 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/07/07 15:52:40 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/Users/mbaioumy/Documents/WebServ/include/Resources.hpp"

int main()
{
    Resources   resources;

    resources.checkRequest("POST /api/user HTTP/1.1\r\nHost: website.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\r\nContent-Type: application/json\r\nContent-Length: 80\r\n\r\n{\"username\": \"<script>alert('XSS attack!');</script>\", \"password\": \"secretpass\"}");
    return (0);
}