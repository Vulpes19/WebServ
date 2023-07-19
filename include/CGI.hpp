#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include "Libraries.hpp"
#include "Resources.hpp"
#include "Response.hpp"

class Resources;

class CGI {
    public:
        CGI( void );
        // CGI( const CGI &src );
        CGI &operator = (const CGI &rhs);
        ~CGI( void );
        // std::string	getCGI( void );
        bool            checkExtension();
        void            setEnv();
        void            getEnvAsArray();
        bool            exec();
        bool            checkCGI(std::string path);
        std::ifstream   getOutFile();
        size_t          getOutFileSize();
        std::string     getStatusCode();
        void            setURI(std::string path);
        void            setEnvirement(Resources &resources);
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
};