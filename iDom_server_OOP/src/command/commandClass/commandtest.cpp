#include "commandtest.h"
#include "../../functions/functions.h"
#include "../../../ftplibpp/ftplib.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{


    std::string msg="test - for test ";
     my_data->main_iDomTools->runOnSunrise();



    return msg;
}

std::string commandTEST::help()
{
    return "test - for test\n";
}
