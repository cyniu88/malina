#include "command_help.h"
#include <vector>
#include <algorithm>

command_help::command_help(const std::string &name,std::map <std::string, std::unique_ptr<command> >* commandMapPtr):
    command(name),
    commandMapPtr(commandMapPtr)
{
}

std::string command_help::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string result = "";
    if (v.size() == 2){

        if (commandMapPtr->find(v[1]) == commandMapPtr->end()){
            return "unknown command: "+ v[1] +" help note not found";
        }
        else{
            return commandMapPtr->find(v[1])->second->help();
        }
    }
    else
    {
        for( auto iter = commandMapPtr->begin();iter not_eq commandMapPtr->end(); ++iter ) {

            result.append(iter->second->help());
            result.append("------------------------------------\n");
        }
    }
    return result;
}

std::string command_help::help() const
{
    return "help - show help for all command \nhelp <parameter> - show help for command\n";
}
