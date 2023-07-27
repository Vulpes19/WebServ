#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>

int	main()
{
	if ( access("/Users/abaioumy/Documents/WebServ/assets/error_images/imgs/Internal_Server_Error.gif", F_OK) == -1 )
		std::cout << "File does not exist" << std::endl;
	else
		std::cout << "File exists" << std::endl;
}