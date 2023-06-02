/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resources.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 14:37:35 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/02 16:57:47 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Resources.hpp"

Resources::Resources( void )
{}

Resources::~Resources( void )
{}

Resources::Resources( const Resources &src )
{
    *this = src;
}

Resources &Resources::operator=( const Resources &rhs )
{
    if ( this != &rhs )
    {
        this->method = rhs.method;
        this->responseHeader = rhs.responseHeader;
        this->fileContentBuffer = rhs.fileContentBuffer;
        this->fileSize = rhs.fileSize;
        this->error = rhs.error;
    }
    return (*this);
}

void    Resources::checkRequest( std::string request )
{
    std::ostream oss;
    std::string end;
    std::string requestMethod, requestURL, requestHTTP, requestHost;
    oss << request;
    oss >> method >> requestURL >> requestHTTP >> end >> requestHost;
    //to check the host header

    //checking methods
    if ( requestMethod == "GET" )
        method = "GET";
    else if ( requestMethod == "POST" )
        method = "POST";
    else if ( requestMethod == "DELETE" )
        method = "DELETE";
    else
        error = METHOD_NOT_ALLOWED;
}

void    Resources::setError( enum Error_code error )
{
    this->error = error;
}

void    Resources::setResponseHeader( void )
{
    
}