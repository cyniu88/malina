#include "command_light.h"

command_light::command_light(const std::string &name):command(name)
{

}

std::string command_light::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command light - wrong paramiter:\n "+ help();
    if (v.at(1) == "info"){
        str_buf = my_data->main_house_lighting_handler->getAllInfoJSON().dump(4);
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
    else{
        str_buf.erase();
        str_buf.append(v.at(1));

        std::replace( str_buf.begin(), str_buf.end(), '\n', ';');
        my_data->main_house_lighting_handler->executeCommandFromMQTT(str_buf);
        //TODO temporary added viber notifiction
        my_data->main_iDomTools->sendViberMsg(str_buf,my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender + "-light");
    }
    return str_buf;
}

std::string command_light::help() const
{
    std::stringstream ret;
    ret << "info - get bulb info" << std::endl;
    ret << "bulb on/off <bulbID> - turn on/off bubl" << std::endl;
    return ret.str();
}
