#include <iostream>
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
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

std::string generateLargeRequestBody(int sizeInBytes) {
    std::string requestBody;
    requestBody.reserve(sizeInBytes);
    
    for (int i = 0; i < sizeInBytes; i++) {
        requestBody += 'A';  // You can modify the character or pattern as needed
    }
    
    return requestBody;
}

int main() {
    int requestSizeLimit = 4000;  // Modify this to match your server's request size limit
    std::ofstream file;
    file.open("testSizeFile");
    
}
