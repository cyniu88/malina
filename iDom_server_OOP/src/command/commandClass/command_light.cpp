#include "command_light.h"

command_light::command_light(const std::string &name):command(name)
{

}

std::string command_light::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command light - wrong paramiter:\n "+ help();
    if (v.size() > 1){
        str_buf.erase();
        for (auto& a : v){
            str_buf.append(" ");
            str_buf.append(a);
        }
        my_data->main_iDomTools->sendViberMsg(str_buf,my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender + "OSWIETLENIE");
    }
    return str_buf;
}

std::string command_light::help() const
{
    return "to be done";
}
