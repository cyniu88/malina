#include "commandtest.h"
#include "../../functions/functions.h"
#include "../../../ftplibpp/ftplib.h"

commandTEST::commandTEST(const std::string &name):command(name)
{
}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{

    if (v[1] == "test")
    {
        std::string msg = "test - for test";
        my_data->main_iDomTools->runOnSunrise();
        return msg;
    }
    else if (v[1] == "throw")
    {
        throw 888;
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
