#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string>

int main(int argc, char *argv[])
{

    std::cout << "START" <<std::endl;
    int ret = 0;

    if (argc == 1)
    {
        std::cout << "nie ma parametru  wiec odpalam "<< std::endl;
        ret = system("/home/pi/programowanie/build-reload-clang-Release/reload reload");
        std::cout << "system() zwraca ret " << ret <<std::endl;

        std::cout << "koncze program bez reload "<<std::endl;
        //sleep(10);
        return 0;
    }

    std::cout << "kończe program z reload i zwracam " << ret <<std::endl;
    //throw "fuck!";
    return ret;
}
