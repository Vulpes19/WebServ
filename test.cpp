#include <iostream>

using namespace std;

int main( void )
{
    std::string str = "Hello there Ayman";

    if ( str.find("there") != str.npos )
        std::cout << "found it\n";
}