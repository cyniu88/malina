
#include <signal.h>
#include <chrono>
#include <iostream>
#include "iDom_server_OOP.h"

using namespace std::chrono_literals;

int main(int argc, char *argv[])
{
    iDomStateEnum iDomStateProgram = iDomStateEnum::WORKING;
    std::cout << "startujemy program iDom" << std::endl;

    if (argc == 1)
    {
        auto t = 5s;
        do
        {
            try
            {
                iDomStateProgram = iDom_main();
                std::cout << "1 zamykam program z : " << iDomStateProgram << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "złąpano wyjatek programu wiec restart " << e.what() << std::endl;
                iDomStateProgram = iDomStateEnum::RELOAD;
            }
            if (iDomStateProgram == iDomStateEnum::RELOAD)
            {
                std::cout << std::endl
                          << "przeładowywuje program" << std::endl;
                std::this_thread::sleep_for((++t));
            }
        } while (iDomStateProgram == iDomStateEnum::RELOAD);

        if (iDomStateProgram == iDomStateEnum::CLOSE)
        {
            std::cout << "zamykam program CLOSE" << std::endl;
            // std::exit(0);
            return 1024;
        }
        else if (iDomStateProgram == iDomStateEnum::RASPBERRY_RELOAD)
        {
            std::cout << "zamykam program i robie restart maliny" << std::endl;
            std::exit(10);
        }
        else if (iDomStateProgram == iDomStateEnum::ERROR)
        {
            std::cout << "zamykam program z ERROREM" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if (iDomStateProgram == iDomStateEnum::HARD_RELOAD)
        {
            std::exit(3);
        }
    }
    else
    {
        int counter = 0;
        int ret = 9;
        while (ret != 0)
        {
            std::this_thread::sleep_for(1s);
            std::cout << "nie ma parametru wiec odpalam program " << ret << std::endl;
            auto p1 = std::chrono::system_clock::now();
            auto timeStart = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
            
            std::cout << "cyniu: " << std::system("pwd") << std::endl;
            ret = std::system("./iDom_server-CMAKE");
            auto p2 = std::chrono::system_clock::now();
            auto timeStart2 = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

            std::cout << "timeStart: " << timeStart << " timeStart2: " << timeStart << " RÓZNICA " << timeStart2 - timeStart << std::endl;
            if ((timeStart2 - timeStart) < 10)
            {
                counter++;
            }
            else
                counter = 0;

            std::cout << "system() zwraca ret " << ret << " | counter restartow: " << counter << std::endl;

            if (ret == 2560)
            {
                std::cout << "reboot maliny " << system("shutdown -r now") << std::endl;
            }

            if (counter == 13)
                break;
        }
        std::cout << "ZAMYKAM NA AMEN" << std::endl;
    }
    return 0;
}
