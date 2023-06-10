/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:27:52 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/10 16:36:14 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Resources.hpp"

Client::Client( void )
{
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
}

Client::~Client( void )
{}

ClientInfo   *Client::getClient( SOCKET socket, Server srv )
{
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() == socket )
            return (*it);
    }
    ClientInfo *newClient = new ClientInfo();
    newClient->reset();
    newClient->createClient( srv.getListenSocket() );
    clients.push_back(newClient);
    return (newClient);
}

void    Client::deleteClient( ClientInfo *cl )
{
    // close(cl->socket);
    iterator it = clients.begin();
    while ( it != clients.end() )
    {
        if ( (*it)->getSocket() == cl->getSocket() )
            break ;
        ++it;
    }
    if ( it != clients.end() )
        clients.erase(it);
    else
        std::cerr << "dropped client not found\n";
}

// const char  *Client::getAddress( ClientInfo *ci )
// {
//     static char address_buffer[100];
//     getnameinfo((struct sockaddr *) &ci->getAddress(), ci->getAddressLen(), address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
//     return (address_buffer);
// }

// void    Client::checkClients( fd_set &readfds, fd_set &writefds, SOCKET srvSocket )
// {
//     SOCKET maxSocket = srvSocket;
//     for ( iterator it = clients.begin(); it != clients.end(); ++it )
//     {
//         FD_SET((*it)->getSocket(), &readfds);
//         if ( (*it)->getSocket() > maxSocket )
//         {
//             std::cout << "added the socket\n";
//             maxSocket = (*it)->getSocket();
//         }
//     }
// }

void  Client::setsManager( SOCKET socket, fd_set &readfds, fd_set &writefds )
{
    FD_ZERO(&writefds);
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);
    SOCKET maxSocket = socket;
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( (*it)->getSocket() > -1 )
        {
            FD_SET((*it)->getSocket(), &readfds);
            if ( (*it)->getState() == WRITE_RESPONSE )
                (*it)->changeSet( readfds, writefds );
            if ( (*it)->getSocket() > maxSocket )
            {
                std::cout << "added the socket\n";
                maxSocket = (*it)->getSocket();
            }
        }
    }
    if ( select( maxSocket + 1, &readfds, &writefds, NULL, &timeout) < 0 )
    {
        std::cerr << "select() failed: " << strerror(errno) << std::endl;
        exit(1);
    }
}

void    Client::errorBadRequest( ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 400 Bad Request\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 11\r\n\r\nBad Request";
    send( cl->getSocket(), errorMsg, strlen(errorMsg), 0 );
    deleteClient( cl );
}

void    Client::errorNotFound( ClientInfo *cl )
{
    const char *errorMsg = "HTTP/1.1 404 Not Found\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 9\r\n\r\nNot Found";
    send( cl->getSocket(), errorMsg, strlen(errorMsg), 0);
}

// const char  *Client::getFileType( const char *path ) const
// {
//     const char *fileName = strrchr(path, '.');
//     if ( strcmp(fileName, ".css") == 0 )
//         return ("text/css");
//     if ( strcmp(fileName, ".mp4") == 0 )
//         return ("video/mp4");
//     if ( strcmp(fileName, ".csv") == 0 )
//         return ("text/csv");
//     if ( strcmp(fileName, ".gif") == 0 )
//         return ("image/gif");
//     if ( strcmp(fileName, ".htm") == 0 || strcmp(fileName, ".html") == 0 )
//         return ("text/html");
//     if ( strcmp(fileName, ".ico") == 0 )
//         return ("image/x-icon");
//     if ( strcmp(fileName, ".jpeg") == 0 || strcmp(fileName, ".jpg") == 0 )
//         return ("image/jpeg");
//     if ( strcmp(fileName, ".js") == 0 )
//         return ("application/javascript");
//     if ( strcmp(fileName, ".json") == 0 )
//         return ("application/json");
//     if ( strcmp(fileName, ".png") == 0 )
//         return ("image/png");
//     if ( strcmp(fileName, ".pdf") == 0 )
//         return ("application/pdf");
//     if ( strcmp(fileName, ".svg") == 0 )
//         return ("image/svg+xml");
//     if ( strcmp(fileName, ".txt") == 0 )
//         return ("text/plain");
//     return "text/plain";
// }

// void    Client::serveResource( ClientInfo *cl, std::string path )
// {
//     //gotta send the response a little by little
//     std::cout << "entering serveResources\n";
//     if ( strcmp(path.c_str(), "/") == 0 )
//     {
//         std::cout << "path is /" << std::endl;
//         path = "/jake.mp4";
//     }
//     if ( path.length() > 100 )
//     {
//         errorBadRequest(cl);
//         return ;
//     }
//     if ( strstr(path.c_str(), "..") )
//     {
//         errorNotFound(cl);
//         return ;
//     }
//     std::ostringstream oss;
//     oss << "public" << path;
//     std::string fullPath = oss.str();
//     std::cout << fullPath << std::endl;
//     std::ifstream file(fullPath.c_str());
//     if ( !file.is_open() )
//     {
//         errorNotFound(cl);
//         return ;
//     }
//     int fileSize = getFileSize(fullPath.c_str());
//     if ( fileSize == -1 )
//         return ;
//     const char *ct = getFileType(fullPath.c_str());
//     oss.str("");
//     oss.clear();
//     char buffer[BSIZE];
//     oss << "HTTP/1.1 200 OK\r\n";
//     oss << "Connection: close\r\n";
//     oss << "Content-Length: " << fileSize << "\r\n";
//     oss << "Content-Type: " << ct << "\r\n";
//     oss << "\r\n";
//     if ( send( cl->getSocket(), oss.str().c_str(), oss.str().size(), 0) == -1 )
//         std::cerr << "send() failed: " << strerror(errno) << std::endl;
//     while (!file.eof())
//     {
//         file.read(buffer, 10);
//         std::streamsize bytesRead = file.gcount();
//         if ( bytesRead > 0 )
//             send(cl->getSocket(), buffer, bytesRead, 0);
//     }
//     file.close();
//     deleteClient(cl);
//     std::cout << "exiting serveResources\n";
// }

// void    Client::readyToWrite( SOCKET socket, fd_set &readfds )
// {
//     FD_CLR(socket, &readfds);
//     FD_SET(socket, &writefds);
// }

// void    Client::readyToRead( SOCKET socket, fd_set &readfds )
// {
//     FD_CLR(socket, &writefds);
//     FD_SET(socket, &readfds);
// }

void    Client::multiplexing( fd_set &readfds, fd_set &writefds )
{
    std::cout << "entering multiplexing\n";
    for ( iterator it = clients.begin(); it != clients.end(); ++it )
    {
        if ( FD_ISSET( (*it)->getSocket(), &readfds) )
        {
            if ( (*it)->getState() == READ_REQUEST )
            {
                (*it)->handleReadRequest();
                // (*it)->setState(PROCESS_REQUEST);
                continue ;
            }
        }
        // if ( (*it)->getState() == PROCESS_REQUEST )
        // {
        //     std::cout << "HELLOOOOOOOOO\n";
        //     if ( (*it)->handleProcessRequest() )
        //         (*it)->changeSet(readfds, writefds);
        //     continue ;
        //     // (*it)->setState(WRITE_RESPONSE);
        // }
        if ( FD_ISSET( (*it)->getSocket(), &writefds) )
        {
            if ( (*it)->getState() == WRITE_RESPONSE )
            {
                if ( (*it)->handleWriteResponse() )
                {
                    (*it)->reset();
                    // deleteClient((*it));
                    (*it)->changeSet(writefds, readfds);
                }
                // if ( clients.empty() )
                //     break ;
                // (*it)->setState(READ_REQUEST);
                continue ;
            }
        }
        std::cout << "exiting multiplexing\n";
    }
    // Resources r;
    // std::cout << "entering checkClients\n";
    // for ( iterator it = clients.begin(); it != clients.end(); ++it )
    // {
    //     std::cout << "client socket: " << (*it)->socket << std::endl;
    //     std::cout << "isReading: " << (*it)->isReading << std::endl;
    //     std::cout << "isWriting: " << (*it)->isWriting << std::endl;
    //     if ( FD_ISSET( (*it)->socket, &readfds) )
    //     {
    //         //gotta add a condition for sending the response here
    //         if ( (*it)->isWriting == true )
    //         {
    //             std::cout << "heloooo\n";
    //             serveResource(*it, (*it)->path);
    //             (*it)->isReading = true;
    //             (*it)->isWriting = false;
    //             (*it)->reset();
    //         }
    //         if ( MAX_REQUEST_SIZE <= (*it)->bytesReceived )
    //         {
    //             errorBadRequest(*it);
    //             continue ;
    //         }
    //         int received = recv((*it)->socket, (*it)->request + (*it)->bytesReceived, MAX_REQUEST_SIZE - (*it)->bytesReceived, 0 );
    //         if ( received < 1 )
    //         {
    //             printf(" unexpected disconnect from %s.\n", getAddress(*it));
    //             deleteClient(*it);
    //             break ;
    //         }
    //         else if ( (*it)->isReading == true )
    //         {
    //             (*it)->bytesReceived += received;
    //             (*it)->request[(*it)->bytesReceived] = 0;
    //             std::string str((*it)->request);
    //             r.checkRequest(str);
    //             char *end = strstr((*it)->request, "\r\n\r\n");
    //             if ( end )
    //             {
    //                 //RESPONSE
    //                 (*it)->path = (*it)->request + 4;
    //                 char *end_path = strstr((*it)->path, " ");
    //                 if ( !end_path )
    //                 {
    //                     (*it)->path = NULL;
    //                     errorBadRequest(*it);
    //                 }
    //                 else
    //                 {
    //                     std::cout << "Im here\n";
    //                     (*it)->isWriting = true;
    //                     readyToWrite((*it)->socket, readfds);
    //                     *end_path = 0;
    //                 }
    //             }
    //         }
    //     }
    //     if ( clients.empty() )
    //         break ;
    // }
}