#include "command_voice.h"
#include "../libs/emoji/emoji.h"

command_voice::command_voice(const std::string &name): command(name)
{

}

std::string command_voice::execute(std::vector<std::string> &v, thread_data *my_data)
{
    for(auto &vv : v){
        useful_F_libs::toLower(vv);
    }

    if(v[0] == "włącz" and v.size() > 3){
        if(v[1] == "radio"){
            my_data->main_iDomTools->MPD_play(my_data);
            return "done";
        }
        else if(v[1] == "światło"){
            if(v[3] == "biurze")
                my_data->main_house_room_handler->turnOnAllInRoom("pokoj_E");
            else if( v[3] == "salonie")
                my_data->main_house_room_handler->turnOnAllInRoom("salon");

            return "done";
        }
    }
    else if(v[0] == "wyłącz" and v.size() > 3){
        if(v[1] == "radio"){
            my_data->main_iDomTools->MPD_stop();
            return "done";
        }
        else if(v[1] == "światło"){
            if(v[3] == "biurze")
                my_data->main_house_room_handler->turnOffAllInRoom("pokoj_E");
            else if(v[3] == "salonie")
                my_data->main_house_room_handler->turnOffAllInRoom("salon");
            else if (v[3] == "wszędzie")
                my_data->main_house_room_handler->turnOffAllBulb();
            return "done";
        }
    }

    std::fstream log;
    log.open( "/mnt/ramdisk/command.txt", std::ios::binary | std::ios::in | std::ios::out|std::ios::app );
    log << "MQTT: " << v[0] << std::endl;
    log.close();
    return EMOJI::emoji(E_emoji::WARNING_SIGN) + " unknown command: "+ v[0];
}

std::string command_voice::help() const
{
    return "TODO";
}
