/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaioumy <mbaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 10:16:08 by abaioumy          #+#    #+#             */
/*   Updated: 2023/07/18 12:15:02 by mbaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void    ErrorResponse::errorBadRequest( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 400 Bad Request\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg +=	"Content-Length: 11\r\n\r\nBad Request";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorNotFound( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 404 Not Found\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg +=	"Content-Length: 9\r\n\r\nNot Found";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorForbidden( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 403 Forbidden\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 9\r\n\r\nForbidden";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorInternal( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 500 Internal Server Error\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 14\r\n\r\nInternal error";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorUnauthorized( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 401 Unauthorized\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 12\r\n\r\nUnauthorized";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorMethodNotAllowed( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 405 Method Not Allowed\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 18\r\n\r\nMethod Not Allowed";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorLengthRequired( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 411 Length Required\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 15\r\n\r\nLength Required";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorHTTPVersion( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 26\r\n\r\nHTTP Version Not Supported";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

void    ErrorResponse::errorRequestTooLarge( SOCKET socket )
{
	std::string errorMsg = "HTTP/1.1 413 Request Entity Too Large\r\n";
	errorMsg += "Connection: close\r\n";
	errorMsg += "Content-Length: 24\r\n\r\nRequest Entity Too Large";
	send( socket, errorMsg.data(), errorMsg.size(), 0 );
}

Response::Response( void )
{
	autoIndex = false;
	bytesReceived = 0;
	bytesSent = 0;
	fileSize = 0;
	socket = -1;
	// path = "";
	indexResponse = "";
	bodySize = 0;
	isBody = false;
	uploadPath = "NONE";
}

Response::~Response( void )
{}

void    Response::setSocket( SOCKET socket )
{
	this->socket = socket;
}

enum ResponseStates    Response::handleReadRequest( Resources &resources )
{
	char    request[4096];
	ssize_t bytesRead;
	size_t	lenPos;
	size_t	endPos;
	size_t	delimiter;
	
	if ( !buffer.is_open() )
		buffer.open("readingRequestFile", std::ios::binary);
	bytesRead = read( socket, request, 4096 );
	if ( bytesRead > 0  )
	{
		std::string toCheck(request, bytesRead);
		buffer.write(request, bytesRead);
		lenPos = toCheck.find("Content-Length: ");
		if ( lenPos != std::string::npos )
		{
			lenPos += 16;
			endPos = toCheck.find("\r\n", lenPos);
			if ( endPos != std::string::npos )
			{
				std::string lenStr = toCheck.substr(lenPos, endPos - lenPos);
				if ( !lenStr.empty() )
				{
					bodySize = std::stoul(lenStr);
					if ( bodySize > bodyLimit )
					{
						err.errorRequestTooLarge(socket);
						buffer.close();
						remove("readingRequestFile");
						reset();
						return (RESET);
					}
				}
			}
		}
	std::string end("\r\n\r\n");
	delimiter = toCheck.find(end);
	if ( isBody )
		bytesReceived += bytesRead;
	if ( delimiter != std::string::npos )
	{
		isBody = true;
		std::string bodyStart(toCheck.begin() + delimiter + end.length(), toCheck.end());
		if ( !bodyStart.empty() )
			bytesReceived += bodyStart.length();
	}
	if ( bytesReceived >= bodySize )
	{
		buffer.close();
		resources.checkRequest();
		return (READY_TO_WRITE);	
	}
	return (READING);
	}
	else if ( bytesRead == 0 )
	{
		buffer.close();
		resources.checkRequest();
		return (READY_TO_WRITE);
	}
	else
	{
		buffer.close();
		err.errorBadRequest(socket);
		remove("readingRequestFile");
		reset();
		return (RESET);
	}
}

bool	ResponseHelper::isDirectory( std::string path ) const
{
	struct stat fileInfo;

	if ( stat(path.c_str() , &fileInfo) == 0 )
		return S_ISDIR(fileInfo.st_mode);
	return (false);
}

enum ResponseStates    Response::getResponseDir( std::string path )
{
	std::ostringstream oss;
	std::string fullPath = "." + path;
	std::cout << "fullPath: " << fullPath << "\n";
	if ( !autoIndex )
	{
		err.errorForbidden(socket);
		reset();
		return (RESET);
	}
	if ( access(fullPath.c_str(), F_OK) == -1 )
	{
		err.errorNotFound(socket);
		reset();
		return (RESET);
	}
	if ( access(fullPath.c_str(), R_OK | X_OK) == -1 )
	{
		err.errorForbidden(socket);
		reset();
		return (RESET);
	}
	DIR *dir = opendir(fullPath.c_str());
	if ( dir == NULL )
	{
		err.errorInternal(socket);
		reset();
		return (RESET);
	}
	indexResponse += "<html><body><ul>";
	struct  dirent *entry;	
	while ( (entry = readdir(dir)) != NULL )
	{
		std::string name = entry->d_name;
		if ( name != "." && name != ".." )
		{
			if ( help.isDirectory( "./" + name ) )
				indexResponse += "<li><a href=\"" + path + "/" + name + "/\">" + name + "/</a></li>";
			else
				indexResponse += "<li><a href=\"" + path + "/" + name + "\">" + name + "</a></li>";
		}
	}
	indexResponse += "</ul></body></html>";
	oss << "HTTP/1.1 200 OK\r\n";
	oss << "Connection: close\r\n";
	oss << "Content-Length: " << indexResponse.size() << "\r\n";
	oss << "Content-Type: " << "text/html" << "\r\n";
	oss << "\r\n";
	send(socket, oss.str().data(), oss.str().size(), 0);
	send(socket, indexResponse.data(), indexResponse.size(), 0);
	closedir(dir);
	reset();
	return (RESET);
}

enum ResponseStates    Response::getResponseFile( std::string path )
{
	if ( !file.is_open() )
	{
		std::cout << " begining of response: " << path << std::endl;
		std::ostringstream oss;
		bytesSent = 0;
		bytesReceived = 0;
		// std::string root = getRootPath(path);
		if ( path != "NONE" )
			oss << "." << path;
		else
		{
			err.errorForbidden(socket);
			reset();
			return (RESET);
		}
		std::string fullPath = oss.str();
		if ( fullPath.back() == '/' )
			fullPath = fullPath.substr(0, fullPath.length() - 1 );
		if ( access(fullPath.c_str(), F_OK) == -1 )
		{
			err.errorNotFound(socket);
			reset();
			return (RESET);
		}
		if ( access( fullPath.c_str(), R_OK) == -1 )
		{
			err.errorForbidden(socket);
			reset();
			return (RESET);
		}
		fileSize = help.getFileSize(fullPath.c_str());
		file.open(fullPath.c_str());
		if ( !file.is_open() )
		{
			err.errorInternal(socket);
			reset();
			return (RESET);
		}
		sendResponseHeader( GET, "200 OK", fullPath, NULL );
	}
	char buffer[BSIZE + 1];
	file.read(buffer, BSIZE);
	ssize_t bytesRead = file.gcount();
	buffer[bytesRead] = '\0';
	if ( bytesSent == -1 )
	{
		err.errorInternal(socket);
		bytesSent = 0;
		file.close();
		reset();
		return (RESET);
	}
	else if ( bytesRead > 0 )
	{
		bytesSent += bytesRead;
		send( socket, buffer, bytesRead, 0 );
		if ( bytesSent == fileSize )
			return (RESET);
		return (READING);
	}
	else
	{
		bytesSent = 0;
		file.close();
		reset();
		return (RESET);
	}
}

enum ResponseStates	Response::postUploadFile( Resources &resources )
{
	std::string filePath(resources.getRequest("URL"));
	std::string type = help.getFileType(resources.getRequest("Content-Type"), TYPE_SUFFIX);

	if ( uploadPath != "NONE" )
		help.normalizePath(uploadPath);
	uploadPath += filePath.substr(1).c_str() + type;
	uploadPath = "." + uploadPath;
	if ( rename("requestBody", uploadPath.c_str()) != 0 )
	{
		err.errorInternal(socket);
		reset();
		return (RESET);
	}
	sendResponseHeader( POST, "201 Created", filePath, &resources );
	send( socket, "File created.", 13, 0 );
	reset();
	return (RESET);
}

enum ResponseStates	Response::deleteFile( std::string filePath, Resources &resources )
{
	// std::string filePath(resources.getRequest("URL"));

	// filePath = filePath.substr(1);
	std::cout << filePath << std::endl;
	if ( std::count(filePath.begin(), filePath.end(), '/') > 1 )
		filePath = "." + filePath;
	if ( filePath.back() == '/' )
		filePath = filePath.substr(0, filePath.length() - 1);
	if ( access(filePath.c_str(), F_OK) == -1 )
	{
		err.errorNotFound(socket);
		reset();
		return (RESET);
	}
	if ( access(filePath.c_str(), W_OK) == -1 )
	{
		err.errorForbidden(socket);
		reset();
		return (RESET);
	}
	if ( remove(filePath.c_str()) == 0 )
	{
		sendResponseHeader( DELETE, "200 OK", "", &resources );
		send( socket, "File deleted.", 13, 0 );
	}
	else
		err.errorInternal(socket);
	reset();
	return (RESET);
}

bool    Response::handleWriteResponse( Resources &resources )
{
	// std::string requestString(request);
	enum ResponseStates ret;
	if ( uploadPath == "NONE" )
		uploadPath = getUploadPath(resources.getRequest("URL"));
	autoIndex = help.getAutoIndex(loc, resources.getRequest("URL"));
	if ( resources.getError() != NO_ERROR )
	{
		if ( handleErrors( resources ) )
		{
			resources.clear();
			return (true);
		}
	}
	// std::cout << "URL " << resources.getRequest("URL") << std::endl; 
	std::string path = getRootPath(resources.getRequest("URL"));
	// std::cout << "before " << path << std::endl;
	help.normalizePath(path);
	// std::cout << "after " << path << std::endl;
	if ( path.find("..") != std::string::npos )
	{
		err.errorForbidden(socket);
		resources.clear();
		return (true);
	}
	if ( resources.getRequest("Method") == "GET")
	{
		if ( help.isDirectory("." + path) && path != "/" )
			ret = getResponseDir(path);
		else
			ret = getResponseFile(path);
	}
	if ( resources.getRequest("Method") == "POST" )
	{
		if ( help.isDirectory("." + path) )
			err.errorForbidden(socket);
		else
			ret = postUploadFile(resources);
	}
	if ( resources.getRequest("Method") == "DELETE" )
	{
		if ( help.isDirectory("." + path) )
			err.errorForbidden(socket);
		else
			ret = deleteFile(path, resources);
	}
	if ( ret == READING )
		return (false);
	else
	{
		resources.clear();
		return (true);
	}
}

bool	Response::isRequestReceived( std::string requestStr, ssize_t bytesRead ) const
{
	std::string end("\r\n\r\n");
	(void)bytesRead;
	if ( strcmp( requestStr.c_str() + bytesReceived - end.length(), end.c_str() ) == 0 )
		return (true);
	return (false);
}

bool	Response::handleErrors( Resources &resources )
{
	switch ( resources.getError() )
	{
		case BAD_REQUEST:
			err.errorBadRequest(socket);
			break ;
		case NOT_FOUND:
			err.errorNotFound(socket);
			break ;
		case FORBIDDEN:
			err.errorForbidden(socket);
			break ;
		case INTERNAL_SERVER_ERROR:
			err.errorInternal(socket);
			break ;
		case UNAUTHORIZED:
			err.errorUnauthorized(socket);
			break ;
		case METHOD_NOT_ALLOWED:
			err.errorMethodNotAllowed(socket);
			break ;
		case LENGTH_REQUIRED:
			err.errorLengthRequired(socket);
			break ;
		case HTTP_VERSION_NOT_SUPPORTED:
			err.errorHTTPVersion(socket);
			break ;
		case REQUEST_ENTITY_TOO_LARGE:
			err.errorRequestTooLarge(socket);
			break ;
		default:
			return (false);
	}
	return (true);
}

void	Response::sendResponseHeader( enum METHODS method, std::string statusCode, std::string fileName, Resources *resources )
{
	std::ostringstream oss;
	oss << "HTTP/1.1 " << statusCode << "\r\n";
	oss << "Connection: close\r\n";
	oss << "Date: " << help.getCurrentTime() << "\r\n";
	//cache-control
	//server name
	if ( method == GET )
	{
		oss << "Content-Type: " << help.getFileType(fileName, TYPE_NAME) << "\r\n";
		oss << "Content-Length: "<< fileSize << "\r\n";
	}
	if ( method == POST )
	{
		std::string ret;
		ret = help.getFileLocation(fileName.substr(1).c_str());
		if ( !ret.empty() )
			oss << "Location: " << ret << "\r\n";
		ret = resources->getRequest("Content-Type");
		if ( ret != "NOT FOUND" )
			oss << "Content-Type: " << ret << "\n";
		oss << "Content-Length: " << "13" << "\r\n";
	}
	if ( method == DELETE )
	{
		oss << "Content-Type: text/plain\r\n";
		oss << "Content-Length: 13\r\n";
	}
	oss << "\r\n";
	send( socket, oss.str().data(), oss.str().size(), 0 );
}

void    Response::setLocations( std::vector<Location> loc )
{
    this->loc = loc;
}

void	Response::setName( std::string name )
{
	this->serverName = name;
}

void	Response::setHost( std::string host )
{
	this->host = host;
}

void	Response::setBodySize( ssize_t bodyLimit )
{
	this->bodyLimit = bodyLimit;
}

std::string	Response::getRootPath( std::string path )
{
	if ( path.empty() )
		path = "/";
	if ( path.back() != '/' )
		path += '/';
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( path == "/" && loc[i].getValue() == "/" )
		{
			std::string rootPath = loc[i].getRoot();
			if ( path.find(rootPath) != std::string::npos )
				return (path);
			if ( rootPath.back() != '/' )
				rootPath += '/';
			std::string indexFile = loc[i].getIndex();
			std::string ret = rootPath + path.substr(loc[i].getValue().length());
			if ( indexFile.empty() )
				return (ret);
			else
			{
				if ( loc[i].getValue().length() != path.length() )
					return ( ret );
				else
					return ( ret + indexFile );
			}
		}
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
		{
			std::string rootPath = loc[i].getRoot();
			if ( path.find(rootPath) != std::string::npos )
				return (path);
			if ( rootPath.back() != '/' )
				rootPath += '/';
			std::string indexFile = loc[i].getIndex();
			std::string ret = rootPath + path.substr(loc[i].getValue().length());
			if ( indexFile.empty() )
				return (ret);
			else
			{
				if ( loc[i].getValue().length() != path.length() )
					return ( ret );
				else
					return ( ret + indexFile );
			}
		}
	}
	return (path);
}

std::string	Response::getUploadPath( std::string path )
{
	if ( path.empty() )
		path = "/";
	if ( path.back() != '/' )
		path += '/';
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( (path == "/" || path.find("/") != std::string::npos) && loc[i].getValue() == "/" )
			return (loc[i].getUpload());
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
			return (loc[i].getUpload());
	}
	return ("NONE");
}

void	Response::reset( void )
{
	// memset(request, 0, sizeof(request));
	bytesReceived = 0;
	bytesSent = 0;
	fileSize = 0;
	socket = -1;
	// path.clear();
	indexResponse.clear();
}

ssize_t  ResponseHelper::getFileSize( const char *path ) const
{
    struct stat fileStat;
    if ( stat(path, &fileStat) == 0 )
        return (fileStat.st_size);
    return (-1);
}

const std::string  	ResponseHelper::getFileType( std::string path, enum TYPES type ) const
{
    // const char *fileName = strrchr(path, '.');
	static std::map< std::string, std::string > fileTypes;
	if ( fileTypes.empty() )
	{
		fileTypes[".css"] = "text/css";
		fileTypes[".mp4"] = "video/mp4";
		fileTypes[".png"] = "image/png";
		fileTypes[".zip"] = "application/zip";
		fileTypes[".csv"] = "text/csv";
		fileTypes[".gif"] = "image/gif";
		fileTypes[".htm"] = "text/.html";
		fileTypes[".html"] = "text/html";
		fileTypes[".ico"] = "image/x-icon";
		fileTypes[".jpeg"] = "image/jpeg";
		fileTypes[".jpg"] = "image/jpeg";
		fileTypes[".js"] = "application/javascript";
		fileTypes[".json"] = "application/json}";
		fileTypes[".pdf"] = "application/pdf";
		fileTypes[".svg"] = "image/svg+xml";
		fileTypes[".txt"] = "text/plain";
	}
	if ( type == TYPE_NAME )
	{
		size_t pos = path.find_last_of('.');
		std::string fileName = path.substr(pos);
		std::map< std::string, std::string >::iterator it = fileTypes.find(fileName);
		if ( it != fileTypes.end() )
			return ( it->second );
		else
			return ("text/plain");
	}
	else
	{
		std::map< std::string, std::string >::iterator it = fileTypes.begin();
		for ( ; it != fileTypes.end(); ++it )
		{
			if ( it->second == path.substr(0, path.length() - 1) )
				return (it->first);
		}
		return ("text/plain");
	}
}

const std::string	ResponseHelper::getCurrentTime( void ) const
{
    std::time_t now = std::time(NULL);
    std::string dateTime = std::ctime(&now);
    dateTime.erase(dateTime.length() - 1);
    return (dateTime);
}

const std::string	ResponseHelper::getFileLocation( const char *relativePath ) const
{
	char path[PATH_MAX];

	char *absolutePath = realpath( relativePath, path );
	if ( absolutePath != NULL)
		return (absolutePath);
	else
		return ("");
}

void	ResponseHelper::normalizePath( std::string &path )
{
	bool prevWasSep = false;

	for ( size_t i = 0; i < path.length(); i++ )
	{
		if ( path[i] == '/' )
		{
			if ( prevWasSep )
			{
				path.erase(i, 1);
				i--;
			}
			else
				prevWasSep = true;
		}
		else
			prevWasSep = false;		
	}
	while ( !path.empty() && path[path.length() - 1] == '/' )
		path.erase(path.size() - 1);
	if ( !path.empty() && path[0] != '/' )
		path = '/' + path;
	if ( !path.empty() && path[path.length() - 1] != '/' )
		path += '/';
}

bool	ResponseHelper::getAutoIndex( std::vector<Location> &loc, std::string path ) const
{
	for ( size_t i = 0; i < loc.size(); i++ )
	{
		if ( path == "/" && loc[i].getValue() == "/" )
			return (loc[i].getAutoIndex());
		if ( path.find(loc[i].getValue()) != std::string::npos && loc[i].getValue() != "/" )
			return (loc[i].getAutoIndex());
	}
	return (false);
}