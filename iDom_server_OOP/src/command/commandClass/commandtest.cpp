#include "commandtest.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string msg="test - for test ";
    std::string txt = my_data->main_iDomTools->getTextToSpeach();
    if (v.size() >1){
    msg += my_data->main_iDomTools->postOnFacebook(v[1]+" \n"+ txt);
    msg += " viber: ";
    msg += my_data->main_iDomTools->sendViberMsg(v[1]+" \n",
            my_data->server_settings->viberReceiver,
            my_data->server_settings->viberSender);
    }
    return msg;
}

std::string commandTEST::help()
{
    return "test - for test\n";
}
