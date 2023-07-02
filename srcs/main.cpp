/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 22:51:50 by mbaioumy          #+#    #+#             */
/*   Updated: 2023/06/26 22:51:50 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Resources.hpp"

int main() {

    Resources res;
    std::string request = "POST /example HTTP/1.1\r\nHost: example.com\r\nContent-Type: text/plain\r\nContent-Length: 24\r\n\r\nHello\r\nWorld!\r\n";
    
    res.checkRequest(request);
    return (0);
}