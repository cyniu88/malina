#include "command_idom.h"

command_iDom::command_iDom(std::string name):command(name)
{

}

std::string command_iDom::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v[1]=="speakers")
    {
        if (v[2] =="ON"){
            my_data->main_iDomTools->turnOnSpeakers();
            return "speakers ON";
        }
        else if (v[2]=="OFF"){
            my_data->main_iDomTools->turnOffSpeakers();
            return "speakers OFF";
        }
        else{
            return "unknow speakers action: "+v[2];
        }
    }

    return " unknown command "+ v[1];
}

std::string command_iDom::help()
{
    std::string ret = "iDom - for control smart home";
    return ret;
}
