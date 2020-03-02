#include "command_event.h"

command_event::command_event(const std::string &name) :command(name)
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
    if (v.size() == 3 && v[2] == "clear"){
        my_data->myEventHandler.run(v[1])->clearEvent();
        return "event " +v[1]+ " has been cleared!";
    }
    if (v.size() == 3 && v[2] == "intensity"){
        std::stringstream intensity;
        intensity << my_data->myEventHandler.run(v[1])->getLast1minNumberEvent();
        return "event " +v[1]+" "+ intensity.str() +" intensity per last minute!";
    }
    if (v.size() == 4 && v[1] == "add"){
         my_data->myEventHandler.run(v[2])->addEvent(v[3]);
        return "event " +v[1]+" " +" added!";
    }
    if (v.size() == 5 && v[2] == "clear"){
        int from = std::stoi(v[3]);
        int to = std::stoi(v[4]);
        my_data->myEventHandler.run(v[1])->clearEvent(from, to);
        return "event " +v[1]+ " has been cleared!";
    }
    return my_data->myEventHandler.help();
}

std::string command_event::help() const
{
    std::stringstream help;
    help << "event- show actual event (all)" << std::endl;
    help << "event <name> - show one event" << std::endl;
    help << "event <name> clear - clear event <name>" << std::endl;
    help << "event <name> intensity - intensity per minute" <<std::endl;
    return help.str();
}
