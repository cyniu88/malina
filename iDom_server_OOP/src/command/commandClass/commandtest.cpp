#include "commandtest.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string msg="test - for test ";

    if (v.size() >1){
    msg += my_data->main_iDomTools->postOnFacebook(v[1]+" to jest post z serwera iDom");
    }
    return msg;
}

std::string commandTEST::help()
{

    return "test - for test\n";
}
