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
        void            setEnv(Resources &resources);
        void            getEnvAsArray();
        bool            exec(Resources &resources, std::string path);
        bool            execCGI(Resources &resources, std::string path, std::ifstream &file);
        std::ifstream   getOutFile();
        size_t          getOutFileSize();
        std::string     getStatusCode();
    private:
        char            **envArray;
        std::map<std::string, std::string> env;
        std::ifstream   oFile;
        std::string     statusCode;
        std::string     fileExtension;
        std::string		path;
        std::string		method;
        std::string		outFile;
        std::string		fileName;
        std::string     outFileStr;
};