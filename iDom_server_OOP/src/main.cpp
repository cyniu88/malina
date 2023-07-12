
#include <signal.h>
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
                std::cout << "zamykam program z : " << iDomStateProgram << std::endl;
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
            std::cout << "zamykam program" << std::endl;
            std::exit(EXIT_SUCCESS);
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
            ret = system("./iDom_server-CMAKE");
            if (ret != 100)
                counter = 0;
            else
                counter++;
            std::cout << "system() zwraca ret " << std::hex << ret << " | counter restartow: " << counter << std::endl;

            if (ret == 2560)
            {
                std::cout << "reboot maliny " << system("shutdown -r now") << std::endl;
            }

            if (counter == 10)
                break;
        }
        std::cout << "ZAMYKAM NA AMEN" << std::endl;
    }
    return 0;
}
