#include "command_program.h"
#include "../../functions/functions.h"

command_program::command_program(std::string name):command(name)
{
}

std::string command_program::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string ret = help();
    if (v.size() <2 )
    {
        return "what?\n" + help();;
    }
    if (v[1] == "stop")
    {
        std::string s ="close server";
        useful_F::send_to_arduino_clock(my_data, "STOP");
        iDomTOOLS::MPD_stop();
        my_data->iDomProgramState = iDomStateEnum::CLOSE;
        my_data->main_iDomTools->saveState_iDom();
        throw s;
    }
    if (v.size() < 3 )
    {
        return "add more paramiters";
    }
    if(v[1] == "reload" && v[2] == "soft"){
        std::string s ="close server";
        useful_F::send_to_arduino_clock(my_data, "RELO");
        iDomTOOLS::MPD_stop();
        my_data->iDomProgramState = iDomStateEnum::RELOAD;
        my_data->main_iDomTools->saveState_iDom();
        throw s;
    }
    else if(v[1] == "reload" && v[2] == "hard")
    {
        std::string s ="close server";
        useful_F::send_to_arduino_clock(my_data, "UPAD");
        iDomTOOLS::MPD_stop();
        my_data->iDomProgramState = iDomStateEnum::HARD_RELOAD;
        my_data->main_iDomTools->saveState_iDom();
        throw s;
    }
    else if(v[1] == "clear" && v[2] == "ram")
    {
        system("sync; echo 3 > /proc/sys/vm/drop_caches");
        ret = "ram has beed freed";
    }
    else
    {
        ret = " what? - "+ v[1];
    }
    return ret;
}

std::string command_program::help()
{
    std::stringstream help;
    help << "program stop - close iDom server"<< std::endl;
    help << "program reload soft - reload iDom server" << std::endl;
    help << "program reload hard - reload iDom server" << std::endl;
    help << "program clear ram   - reload iDom server" << std::endl;
    return help.str();
}
