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
    std::string dir = "./videospublic/
}

// int main( void )
// {
//     std::string p = "video.mp4";
//     // char *f = strrchr(p, '.');
    
//     std::cout << p.substr(5) << std::endl;
// }