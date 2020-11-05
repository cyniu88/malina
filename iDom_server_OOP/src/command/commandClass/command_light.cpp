#include "command_light.h"

command_light::command_light(const std::string &name):command(name)
{
}

std::string command_light::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command light - wrong paramiter:\n "+ help();
    if (v.at(1) == "info" && v.size() < 3){
        str_buf = my_data->main_house_lighting_handler->getAllInfoJSON().dump(4);
    }
    else if (v.at(1) == "info" && v.at(2) == "on"){
        str_buf = my_data->main_house_lighting_handler->getInfoJSON_allON().dump(4);
    }
    else if(v.at(1) == "bulb"){
        int bulbID = std::stoi(v.at(3));
        if(v.at(2) == "on"){
            my_data->main_house_lighting_handler->turnOnBulb(bulbID);
        }
        else if(v.at(2) == "off"){
            my_data->main_house_lighting_handler->turnOffBulb(bulbID);
        }

        return "done";
    }
    else if(v.at(1) == "room"){
        if(v.at(3) == "on"){
            my_data->main_house_lighting_handler->turnOnAllInRoom(v.at(2));
        }
        else if(v.at(3) == "off"){
            my_data->main_house_lighting_handler->turnOffAllInRoom(v.at(2));
        }

        return "done";
    }
    else if(v.at(1) == "all"){
        if(v.at(2) == "on"){
            my_data->main_house_lighting_handler->turnOnAllBulb();
        }
        else if(v.at(2) == "off"){
            my_data->main_house_lighting_handler->turnOffAllBulb();
        }

        return "done";
    }
    else{
        str_buf.erase();
        str_buf.append(v.at(1));
        std::replace( str_buf.begin(), str_buf.end(), '\n', ';');
        my_data->main_house_lighting_handler->executeCommandFromMQTT(str_buf);
    }
    return str_buf;
}

std::string command_light::help() const
{
    std::stringstream ret;
    ret << "light info - get bulb info" << std::endl;
    ret << "light info on - get bulb info only on" << std::endl;
    ret << "light bulb on/off <bulbID> - turn on/off bulb" << std::endl;
    ret << "light room <room name> on/off - turn on/off all bulbs in room" << std::endl;
    ret << "light all on/off - turn on/off all bulbs in house" << std::endl;
    return ret.str();
}
