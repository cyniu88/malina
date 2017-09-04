#include "command_event.h"

command_event::command_event(std::string name) :command(name)
{

}

std::string command_event::execute(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size() == 1){
        return my_data->myEventHandler.getListPossibleEvents();
    }
    if (v.size() == 2){
            return my_data->myEventHandler.run(v[1])->getEvent();
    }
    return my_data->myEventHandler.help();
}

std::string command_event::help()
{
    std::string help = "event- show actual  event (all)\n";
    help.append("event <name> - show one event");
    return help.append("\n");
}
