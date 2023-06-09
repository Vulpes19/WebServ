#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

int main( void )
{
    ifstream file("./public/index.html");
    if ( !file.is_open() )
    {
        cerr << "bla bla\n";
        return (1);
    }
    char buffer[11] = {0};
    while ( !file.eof() )
    {
        file.read(buffer, 10);
        write(1, buffer, file.gcount());
    }
    cout.flush();
    file.close();
}