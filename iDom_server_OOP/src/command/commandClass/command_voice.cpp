#include "command_voice.h"

command_voice::command_voice(const std::string &name): command(name)
{

}

std::string command_voice::execute(std::vector<std::string> &v, thread_data *my_data)
{
    for(auto &vv : v){
        useful_F_libs::toLower(vv);
    }
    std::cout <<"Mqtt: ";
    for(auto &vv : v){
        std::cout << vv << std::endl;
    }


    return "TODO";
}

std::string command_voice::help() const
{
    return "TODO";
}
