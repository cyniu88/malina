#include "command_light.h"

command_light::command_light(const std::string &name) : command(name)
{
}

std::string command_light::execute(std::vector<std::string> &v, thread_context *context)
{
    std::string str_buf = "command light - wrong paramiter:\n " + help();
    if (v.size() < 2)
        return str_buf;
    if (v.at(1) == "info" && v.size() < 3)
    {
        str_buf = context->main_house_room_handler->getAllInfoJSON().dump(4);
     }
    else if (v.at(1) == "clean" && v.size() < 3)
    {
        context->main_house_room_handler->turnOffUnexpectedBulb();
        str_buf = "done " + std::to_string(++counter);
    }
    else if (v.at(1) == "info" && v.at(2) == "on")
    {
        str_buf = context->main_house_room_handler->getInfoJSON_allON().dump(4);
    }
    else if (v.at(1) == "bulb")
    {
        int bulbID = std::stoi(v.at(3));
        if (v.at(2) == "on")
        {
            if (v.size() > 4 and v.at(4) == "priority")
                context->main_house_room_handler->m_lightingBulbMap.at(bulbID)->lighting_priority = true;

            context->main_house_room_handler->turnOnBulb(bulbID);
        }
        else if (v.at(2) == "off")
        {
            context->main_house_room_handler->turnOffBulb(bulbID);
        }
        else if (v.at(2) == "change")
        {
            context->main_house_room_handler->turnChangeBulb(bulbID);
        }

        return "done " + std::to_string(++counter);
    }
    else if (v.at(1) == "room")
    {

        if (v.size() == 3 && v.at(2) == "stats")
        {
            std::stringstream msg;
            for (auto &j : context->main_house_room_handler->m_roomMap)
            {
                msg << j.second->getStatsJSON().dump(4) << std::endl;
            }
            return msg.str();
        }
        if (v.at(3) == "on")
        {
            context->main_house_room_handler->turnOnAllInRoom(v.at(2));
        }
        else if (v.at(3) == "off")
        {
            context->main_house_room_handler->turnOffAllInRoom(v.at(2));
        }
        else if (v.at(3) == "change")
        {
            context->main_house_room_handler->changeAllInRoom(v.at(2));
        }
        // TODO dodac rozpoznawanie nazwy pokoju  z wiecej niz jednym wyrazem

        return "done " + std::to_string(++counter);
    }
    else if (v.at(1) == "all")
    {
        if (v.at(2) == "on")
        {
            context->main_house_room_handler->turnOnAllBulb();
        }
        else if (v.at(2) == "off")
        {
            context->main_house_room_handler->turnOffAllBulb();
        }

        return "done " + std::to_string(++counter);
    }
    else
    {
        str_buf.erase();
        str_buf.append(v.at(1));
        std::replace(str_buf.begin(), str_buf.end(), '\n', ';');
        context->main_house_room_handler->executeCommandFromMQTT(str_buf);
    }
    return str_buf;
}

std::string command_light::help() const
{
    std::stringstream ret;
    ret << "light info - get bulb info" << std::endl;
    ret << "light info on - get bulb info only on" << std::endl;
    ret << "light bulb on/off/change <bulbID> <priority>- turn on / off / change bulb, optional priority to avoid auto turn off" << std::endl;
    ret << "light room <room name> on/off/change - turn on / off / change all bulbs in room" << std::endl;
    ret << "light room stats - get all room stats json" << std::endl;
    ret << "light all on/off - turn on/off all bulbs in house" << std::endl;
    ret << "light clean - turn off unexpected bulbs" << std::endl;
    return ret.str();
}
