#include <iostream>
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
using namespace std;

// int main( void )
// {
//     ifstream file("./public/index.html");
//     if ( !file.is_open() )
//     {
//         cerr << "bla bla\n";
//         return (1);
//     }
//     char buffer[11] = {0};
//     while ( !file.eof() )
//     {
//         file.read(buffer, 10);
//         write(1, buffer, file.gcount());
//     }
//     cout.flush();
//     file.close();
// }

// int main( void )
// {
//     std::ifstream file("./public/");
//     std::cout << access("./obj/", F_OK) << std::endl;
//     if ( access("./public/", X_OK) == -1 )
//         std::cerr << strerror(errno) << std::endl;
// }

// std::string getTime( void )
// {
//     std::time_t now = std::time(NULL);
//     std::string dateTime = std::ctime(&now);
//     dateTime.erase(dateTime.length() - 1);
//     return (dateTime);
// }

// int main( void )
// {
//     std::string toPrint = getTime();
//     std::cout << toPrint << endl;
// }

// #include "testheader.hpp"

// int main( int ac, char **av )
// {
//     Parser parser;

//     parser.openFile(av[1]);
//     parser.printData();
// }

// int main( void )
// {
//     struct stat st;
//     if ( stat("./var/www/videos/jake.mp4", &st) == 0 )
//         cout << st.st_size << endl;
//     else
//         cout << "bruh\n";
//     return (0);
// }

int main( void )
{
    std::string dir = "./Users/abaioumy/Documents/webServ/assets/error_images/imgs/Timeout.gif";
    std::ifstream f(dir.c_str());
    if ( f.is_open())
        std::cout << "cant be opened\n";
//     char path[PATH_MAX];
//     char *absolutePath = realpath( dir.c_str(), path );
//     if ( absolutePath != NULL)
//         std::cout << absolutePath << std::endl;
    if ( access("/Users/abaioumy/Documents/webServ/assets/error_images/imgs/Timeout.gif", F_OK) == -1 )
        std::cerr << "nu such file or directory\n";
	if ( access(dir.c_str(), W_OK) == -1 )
        std::cerr << "no write permission\n";
}

// int main( void )
// {
//     std::string p = "video.mp4";
//     // char *f = strrchr(p, '.');
    
//     std::cout << p.substr(5) << std::endl;
// }