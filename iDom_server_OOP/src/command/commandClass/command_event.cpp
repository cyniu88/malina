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
    if (v.size() == 3 && v[2]=="clear"){
        my_data->myEventHandler.run(v[1])->clearEvent();
        return  "event " +v[1]+ " has been cleared!";
    }
    if (v.size() == 5 && v[2]=="clear"){
        unsigned int from = std::stoi(v[3]);
        unsigned int to = std::stoi(v[4]);
        my_data->myEventHandler.run(v[1])->clearEvent(from, to);
        return  "event " +v[1]+ " has been cleared!";
    }
    return my_data->myEventHandler.help();
}

std::string command_event::help()
{
    std::string help = "event- show actual  event (all)\n";
    help.append("event <name> - show one event\n");
    help.append("event <name> clear - clear event <name>");
    return help.append("\n");
}
