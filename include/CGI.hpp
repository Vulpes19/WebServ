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
        // void		setCGI( std::string path, std::string method, std::string body );
        // std::string	getCGI( void );
        bool checkExtension();
        void setEnv(Resources &resources);
        void getEnvAsArray();
        void exec(Resources &resources);
        bool checkCGI(std::string path);
    private:
        std::map<std::string, std::string> env;
        char            **envArray;
        std::string     fileExtension;
        std::string		path;
        std::string		method;
        std::string		body;
        std::string		fileName;
};