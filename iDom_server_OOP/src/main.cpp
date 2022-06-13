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
            }
            catch (const std::exception& e)
            {
                std::cout << "złąpano wyjatek programu wiec restart "<< e.what() << std::endl;
                iDomStateProgram = iDomStateEnum::RELOAD;
            }
            if(iDomStateProgram == iDomStateEnum::RELOAD)
            {
                std::cout<<std::endl << "przeładowywuje program" << std::endl;
                std::this_thread::sleep_for((++t));
            }
        }
        while (iDomStateProgram == iDomStateEnum::RELOAD);

        if(iDomStateProgram == iDomStateEnum::CLOSE)
        {
            std::cout << "zamykam program" << std::endl;
            return 0;
        }
        else if (iDomStateProgram == iDomStateEnum::ERROR)
        {
            std::cout << "zamykam program z ERROREM" << std::endl;
            return 1;
        }
        else if (iDomStateProgram == iDomStateEnum::HARD_RELOAD)
        {
            return 2;
        }
    }
    else
    {
        int ret = 9;
        while (ret != 0)
        {
            std::this_thread::sleep_for(1s);
            std::cout << "nie ma parametru wiec odpalam program "<< std::endl;
            ret = system("./iDom_server-CMAKE");
            std::cout << "system() zwraca ret " << ret <<std::endl;
        }
        std::cout << "ZAMYKAM NA AMEN" << std::endl;
    }
    return 0;
}
