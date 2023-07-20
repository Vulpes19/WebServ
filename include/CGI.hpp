#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "Libraries.hpp"
#include "Resources.hpp"
// #include "Response.hpp"

class Resources;

class CGI {
    public:
        CGI( void );
        // CGI( const CGI &src );
        CGI &operator = (const CGI &rhs);
        ~CGI( void );
        // std::string	getCGI( void );
        bool            checkExtension();
        void            setEnv(Resources &resources, std::string port, std::string serverName);
        void            getEnvAsArray();
        bool            exec();
        bool            checkCGI(Resources &resources, std::string path, std::string port, std::string serverName);
        std::ifstream   getOutFile();
        size_t          getOutFileSize();
        std::string     getStatusCode();
        void            setURI(std::string path);
    private:
        char            **envArray;
        std::map<std::string, std::string> env;
        std::map<std::string, std::string> resources;
        // std::ofstream   oFile;
        std::string     statusCode;
        std::string     fileExtension;
        std::string		URI;
        std::string		method;
        std::string		outFile;
        std::string		fileName;
        std::string     outFileStr;
        std::string     location;
};