#include "commandtest.h"
#include "../../iDomTools/idomtools_interface.h"
#include <signal.h>
#include "../../functions/functions.h"
#include <malloc.h>

commandTEST::commandTEST(const std::string &name) : command(name)
{
}

std::string commandTEST::execute(std::vector<std::string> &v, thread_context *context)
{

    if (v[1] == "test")
    {
        return "test - for test";
    }
    else if (v[1] == "throw")
    {
        throw 888;
    }
    else if (v[1] == "abort")
    {
        puts("podnosimy SEGV");
        raise(SIGSEGV);
    }
    else if (v[1] == "reboot")
    {
        context->main_iDomTools->raspberryReboot();
    }
    else if (v[1] == "memory")
    {
        {
            std::vector<int> vv;
            int count = std::stoi(v[2]);

            for (int i = 0; i < count; ++i)
            {
                vv.push_back(i);
            }
            useful_F::sleep(3s);
            std::cout << "mallocTrim: " << malloc_trim(0) << std::endl;
        }


        return "done";
    }
        else if (v[1] == "memory2")
    {
        {
            std::map<int, int> vv2;
            int count = std::stoi(v[2]);

            for (int i = 0; i < count; ++i)
            {
                vv2.insert({i,i});
            }
            useful_F::sleep(3s);
            std::cout << "mallocTrim: " << malloc_trim(0) << std::endl;
        }


        return "done";
    }
    return help();
}
std::string commandTEST::help() const
{
    std::stringstream help;
    help << "test test - for test" << std::endl;
    help << "test throw - trow test reload program" << std::endl;
    return help.str();
}
