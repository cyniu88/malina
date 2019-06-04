#include "commandhandlergateway.h"
#include "../libs/emoji/emoji.h"

commandHandlerGATEWAY::commandHandlerGATEWAY(thread_data *my_data): commandHandler(my_data)
{

}

commandHandlerGATEWAY::~commandHandlerGATEWAY()
{
    puts("commandHandlerGATEWAY::~commandHandlerGATEWAY()");
}

std::string commandHandlerGATEWAY::run(std::vector<std::string> &v, thread_data *my_data)
{
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
