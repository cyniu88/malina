#include "command_help.h"
#include <vector>
#include <algorithm>

command_help::command_help(const std::string &name):command(name)
{

}

std::string command_help::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string result ="";
    if (v.size() ==2){

        if (my_data->commandMapPtr->find(v[1]) == my_data->commandMapPtr->end()){
            return "unknown command: "+ v[1]+" help note not found";
        }
        else{
            return my_data->commandMapPtr->find(v[1])->second->help();
        }
    }
    else
    {
        for( auto iter= my_data->commandMapPtr->begin();iter != my_data->commandMapPtr->end(); ++iter ) {

            result+= iter->second->help();
            result+= "------------------------------------\n";
        }
    }
    return result;
}

std::string command_help::help()
{
    return "help - show help for all command \nhelp <parameter> - show help for command\n";
}
