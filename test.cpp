#include <iostream>
#include <fstream>
#include <unistd.h>
#include <errno.h>
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

int main( void )
{
    std::ifstream file("./test_bruh");
    std::cout << access("./test_bruh", F_OK) << std::endl;
    if ( access("./test_bruh", R_OK) == -1 )
        std::cerr << strerror(errno) << std::endl;
}