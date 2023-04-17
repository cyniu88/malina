#include "commandhandlergateway.h"
#include "../libs/emoji/emoji.h"
#include "commandClass/command_gateway.h"
#include "../iDomTools/idomtools_interface.h"

commandHandlerGATEWAY::commandHandlerGATEWAY(thread_data *my_data) : commandHandler(my_data)
{
    std::unique_ptr<command> gateway(new command_gateway("gateway"));
    commandMap.insert(std::make_pair(gateway->getCommandName(), std::move(gateway)));
}

std::string commandHandlerGATEWAY::run(std::vector<std::string> &v, thread_data *my_data)
{
    if (v.size() < 3)
    {
        return "not enough parameters";
    }

    my_data->main_iDomTools->sendViberMsg("iDom gateway run command",
                                          my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                          my_data->server_settings->_fb_viber.viberSender + "GATEWAY");

    if (my_data->m_keyHandler->useKEY(v.front(), v[1]) == false)
    {
        return EMOJI::emoji(E_emoji::WARNING_SIGN) + " wrong key! ";
    }

    v.erase(v.begin(), v.begin() + 2);

    if (commandMap.find(v.front()) == commandMap.end())
    {
        std::fstream log;
        log.open("/mnt/ramdisk/command.txt", std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
        log << "MQTT: " << v.front() << std::endl;
        log.close();
        return EMOJI::emoji(E_emoji::WARNING_SIGN) + " unknown command: " + v.front();
    }
    else
    {
        return commandMap[v.front()]->execute(v, my_data);
    }
}
