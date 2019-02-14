#include "commandhandlermqtt.h"
#include "../../libs/emoji/emoji.h"
#include "commandClass/command_433mhz.h"

CommandHandlerMQTT::CommandHandlerMQTT()
{
    std::unique_ptr <command> r_433MHz (new command_433MHz("433MHz"));
    commandMap.insert(std::make_pair(r_433MHz->getCommandName(), std::move(r_433MHz)));
}

std::string CommandHandlerMQTT::run(std::vector<std::string> &v, thread_data *my_data)
{
    if (commandMap.find(v[0]) == commandMap.end()){
        std::fstream log;
        log.open( "/mnt/ramdisk/command.txt", std::ios::binary | std::ios::in | std::ios::out|std::ios::app );
        log << v[0] << std::endl;
        log.close();
        return EMOJI::emoji(E_emoji::WARNING_SIGN)+" unknown command: "+ v[0];
    }
    else{
        return commandMap[v[0]]->execute(v,my_data);
    }
}
