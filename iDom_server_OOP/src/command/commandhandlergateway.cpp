#include "commandhandlergateway.h"
#include "../libs/emoji/emoji.h"
#include "commandClass/command_gateway.h"

commandHandlerGATEWAY::commandHandlerGATEWAY(thread_data *my_data): commandHandler(my_data)
{
    std::unique_ptr <command> gateway (new command_gateway("gateway"));
    commandMap.insert(std::make_pair(gateway->getCommandName(), std::move(gateway)));
}

commandHandlerGATEWAY::~commandHandlerGATEWAY()
{
    puts("commandHandlerGATEWAY::~commandHandlerGATEWAY()");
}

std::string commandHandlerGATEWAY::run(std::vector<std::string> &v, thread_data *my_data)
{
    if(v.size() < 3){
        return "not enough parameters";
    }

    if(my_data->main_iDomTools->m_keyHandler->useKEY(v[0],v[1]) == false)
    {
        return EMOJI::emoji(E_emoji::WARNING_SIGN)+" wrong key! ";
    }

    v.erase(v.begin(),v.begin()+2);

    if (commandMap.find(v[0]) == commandMap.end()){
        std::fstream log;
        log.open( "/mnt/ramdisk/command.txt", std::ios::binary | std::ios::in | std::ios::out|std::ios::app );
        log << "MQTT: " << v[0] << std::endl;
        log.close();
        return EMOJI::emoji(E_emoji::WARNING_SIGN)+" unknown command: "+ v[0];
    }
    else{
        return commandMap[v[0]]->execute(v,my_data);
    }
}
