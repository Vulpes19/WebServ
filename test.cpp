#include <iostream>

using namespace std;

int main( void )
{
    string str = "Hello there Ayman";

    cout << str.find("z") << endl;
    cout << str.npos << endl;
    cout << string::npos << endl;
}