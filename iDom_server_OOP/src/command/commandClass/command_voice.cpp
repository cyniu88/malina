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

    if(vectorContain(v,"włą") or vectorContain(v, "zaś")){
        if(vectorContain(v,"rad")){
            my_data->main_iDomTools->MPD_play(my_data);
            return "done";
        }
        else if(vectorContain(v, "świat")){
            for(auto &j : my_data->main_house_room_handler->m_roomMap){
                if(vectorContain(v, j.first.substr(0,5))){
                    my_data->main_house_room_handler->turnOnAllInRoom(j.first);
                }
            }
            return "done";
        }
        else if(vectorContain(v, "led")){
           (void) my_data->main_iDomTools->ledOn();
        }
        else if(vectorContain(v, "kodi")){
            my_data->main_iDomTools->startKodi_Thread();
            return "kodi run";
        }
    }
    else if(vectorContain(v, "wyłą") or vectorContain(v, "zgaś")){
        if(vectorContain(v, "radi")){
            my_data->main_iDomTools->MPD_stop();
            return "done";
        }
        else if(vectorContain(v, "świat")){
            if(vectorContain(v, "wsz") or vectorContain(v, "dom")){
                my_data->main_house_room_handler->turnOffAllBulb();
                return "done";
            }
            for(auto &j : my_data->main_house_room_handler->m_roomMap){
                if(vectorContain(v, j.first.substr(0,5))){
                    my_data->main_house_room_handler->turnOffAllInRoom(j.first);
                }
            }
            return "done";
        }
        else if(vectorContain(v, "led")){
            (void) my_data->main_iDomTools->ledOFF();
        }
    }
    else if(vectorContain(v, "pomru")){
       // my_data->main_iDomTools
        return "done";
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

bool command_voice::vectorContain(const std::vector<std::string> &vector, const std::string &str)
{
    for(auto &vv : vector){
        if(useful_F_libs::hasSubstring(vv, str) == true)
            return true;
    }
    return false;
}
