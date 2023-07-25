#include <stdio.h>

int main() {
    printf("Status: 200 OK\r\n");
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><body><h1>Hello, CGI World!</h1></body></html>");
    return 0;
}
