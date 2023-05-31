#include "commandtest.h"
#include "../../iDomTools/idomtools_interface.h"
#include <signal.h>

commandTEST::commandTEST(const std::string &name):command(name)
{
}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
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
        raise(SIGABRT);
    }
	else if (v[1] == "reboot")
	{
		my_data->main_iDomTools->raspberryReboot();
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
