#include <time.h>
 
#include <iostream>

int main()
{
    time_t czas;
    struct tm * ptr;
    time( & czas );
    ptr = localtime( & czas );
    char * data = asctime( ptr );
    std::cout << "Data: " << data;

    return 0;
}
