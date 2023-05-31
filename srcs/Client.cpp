/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/31 15:13:42 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void )
{}

Client::~Client( void )
{}

ClientInfo   *Client::getClient( SOCKET socket, Server srv )
{
    iterator it = clients.find(socket);
    if ( it != clients.end() )
        return (it->second);
    ClientInfo *newClient = new ClientInfo();
    newClient->addressLen = sizeof(newClient->address);
    socket = accept( srv.getListenSocket(), (struct sockaddr *) &(newClient->address), &(newClient->addressLen) );
    newClient->socket = socket;
    clients.insert(pair(socket, newClient));
    return (newClient);
}

void    Client::deleteClient( ClientInfo *cl )
{
    iterator it = clients.find(cl->socket);
    if ( it != clients.end() )
        clients.erase(it);
    std::cerr << "dropped client not found\n";
}

const char  *Client::getAddress( ClientInfo *ci )
{
    static char address_buffer[100];
    getnameinfo((struct sockaddr *) &ci->address, ci->addressLen, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    return (address_buffer);
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

void    Client::errorBadRequest( ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 400 Bad Request\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 11\r\n\r\nBad Request";
    send( cl->socket, errorMsg, strlen(errorMsg), 0 );
    deleteClient( cl );
}

void    Client::errorNotFound( ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 404 Not Found\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 9\r\n\r\nNot Found";
    send( cl->socket, errorMsg, strlen(errorMsg), 0);
}

size_t  Client::getFileSize( const char *path )
{
    struct stat fileStat;
    if ( stat(path, &fileStat) == 0 )
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
    return NULL;
}

void    Client::serveResource( ClientInfo *cl, std::string path )
{
    if ( strcmp(path.c_str(), "/") == 0 )
        path = "/index.html";
    if ( path.length() > 100 )
    {
        errorBadRequest(cl);
        return ;
    }
    if ( strstr(path.c_str(), "..") )
    {
        errorNotFound(cl);
        return ;
    }
    std::ostringstream oss;
    oss << "public" << path;
    std::string fullPath = oss.str();
    std::cout << fullPath << std::endl;
    std::ifstream file(fullPath.c_str());
    if ( !file.is_open() )
    {
        errorNotFound(cl);
        return ;
    }
    int fileSize = getFileSize(fullPath.c_str());
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
    if ( getFileType(fullPath.c_str()) )
        oss << "Content-Type: " << getFileType(fullPath.c_str()) << "\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
    oss.clear();
    oss << "\r\n";
    send( cl->socket, oss.str().c_str(), strlen(oss.str().c_str()), 0);
}

void    Client::checkClients( fd_set reads )
{
    int i = 0;
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        std::cout << i << std::endl;
        i++;
        std::cout << "client socket: " << it->second->socket << std::endl;
        if ( FD_ISSET( it->first, &reads) )
        {
            std::cout << "hahaha\n";
            if ( MAX_REQUEST_SIZE == it->second->bytesReceived )
            {
                errorBadRequest(it->second);
                continue ;
            }
            int received = recv(it->first, it->second->request + it->second->bytesReceived, MAX_REQUEST_SIZE - it->second->bytesReceived, 0 );
            if ( received < 1 )
            {
                printf(" unexpected disconnect from %s.\n", getAddress(it->second));
                deleteClient(it->second);
            }
            else
            {
                std::cout << "helo\n";
                it->second->bytesReceived += received;
                it->second->request[it->second->bytesReceived] = 0;
                char *tmp = strstr(it->second->request, "\r\n\r\n");
                if ( tmp )
                {
                std::cout << "hello\n";
                    if ( strncmp("GET /", it->second->request, 5) )
                        errorBadRequest(it->second);
                    else
                    {
                std::cout << "helllo\n";
                        char *path = it->second->request + 4;
                        char *end_path = strstr(path, " ");
                        if ( !end_path )
                            errorBadRequest(it->second);
                        else
                        {
                std::cout << "hellllo\n";
                            *end_path = 0;
                            serveResource(it->second, path);
                        }
                    }
                }
            }
        }
    }
}