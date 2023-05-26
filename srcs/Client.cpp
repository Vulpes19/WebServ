/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/26 14:48:23 by codespace        ###   ########.fr       */
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
    FD_ZERO(&reads);
    FD_SET(socket, &reads);
    SOCKET maxSocket = socket;
    iterator it;
    for ( it = clients.begin(); it != clients.end(); ++it )
    {
        FD_SET(it->first, &reads);
        if ( it->first > maxSocket )
            maxSocket = it->first;
    }
    if ( select( maxSocket + 1, &reads, 0, 0, 0) < 0 )
    {
        std::cerr << "select() failed: " << strerror(errno) << std::endl;
        exit(1);
    }
    return (reads);
}

void    Client::errorBadRequest( struct ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 400 Bad Request\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 11\r\n\r\nBad Request";
    send( cl->socket, errorMsg, strlen(errorMsg), 0 );
    deleteClient( cl );
}

void    Client::errorNotFound( struct ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 404 Not Found\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 9\r\n\r\nNot Found";
    send( cl->socket, errorMsg, strlen(errorMsg), 0);
}

size_t  Client::getFileSize( const char *path )
{
    struct stat fileStat;
    if ( stat(path.c_str(), &fileStat) == 0 )
    {
        std::cout << "file size is " << fileStat.st_size << std::endl;
        return (fileStat.st_size);
    }
    else
        std::cerr << "cant get file size\n";
    return (-1);
}

const char  *Client::getFileType( const char *path ) const
{
    const char *fileName = strrchr(path, '.');
    if ( strcmp(fileName, ".css") == 0 )
        return ("text/css");
    if ( strcmp(fileName, ".csv") == 0 )
        return ("text/csv");
    if ( strcmp(fileName, ".gif") == 0 )
        return ("image/gif");
    if ( strcmp(fileName, ".htm") == 0 || strcmp(fileName, ".html") == 0 )
        return ("text/html");
    if ( strcmp(fileName, ".ico") == 0 )
        return ("image/x-icon");
    if ( strcmp(fileName, ".jpeg") == 0 || strcmp(fileName, ".jpg") == 0 )
        return ("image/jpeg");
    if ( strcmp(fileName, ".js") == 0 )
        return ("application/javascript");
    if ( strcmp(fileName, ".json") == 0 )
        return ("application/json");
    if ( strcmp(fileName, ".png") == 0 )
        return ("image/png");
    if ( strcmp(fileName, ".pdf") == 0 )
        return ("application/pdf");
    if ( strcmp(fileName, ".svg") == 0 )
        return ("image/svg+xml");
    if ( strcmp(fileName, ".txt") == 0 )
        return ("text/plain");
}

void    Client::serveResource( struct ClientInfo *cl, char *path )
{
    if ( strcmp(path, "/") == 0 )
        path = "/index.html";
    if ( strlen(path) > 100 )
    {
        errorBadRequest(cl);
        return ;
    }
    if ( strstr(path, "..") )
    {
        errorNotFound(cl);
        return ;
    }
    std::ostringstream oss;
    oss << "public" << path;
    std::string fullPath = oss.str();
    std::ifstream file(fullPath.c_str());
    if ( !file.is_open() )
    {
        errorNotFound(cl);
        return ;
    }
    size_t fileSize = getFileSize(fullPath.c_str());
    if ( fileSize == -1 )
        return ;
    oss.clear();
    oss << "HTTP/1.1 200 OK\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
    oss.clear();
    oss << "Connection: close\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
    oss.clear();
    oss << "Content-Length: " << fileSize << "\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
    oss.clear();
    oss << "Content-Type: " << getFileType(fullPath.c_str()) << "\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
    oss.clear();
    oss << "\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
}