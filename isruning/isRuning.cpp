#include <iostream>
#include <stdlib.h>
#include <stdio.h>
     
bool isRunning(const char* name)   
{   
    char command[32];
    sprintf(command, "pgrep %s > /dev/null", name);
    return 0 == system(command);
}

int main(int argc, char*argv[])
{
    char * name = argv[1];
    
    std::cout << name << " ";

    if (isRunning(name))
    {
        std::cout << "is running" << std::endl;
    }
    else
    {
        std::cout << "is not running" << std::endl;
    }
    return 0;
}
