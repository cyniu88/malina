#include "commandhandlermqtt.h"
#include "../../libs/emoji/emoji.h"
#include "commandClass/command_433mhz.h"
#include "commandClass/commandrs232.h"
#include "commandClass/command_mpd.h"

CommandHandlerMQTT::CommandHandlerMQTT()
{
    std::unique_ptr <command> r_433MHz (new command_433MHz("433MHz"));
    commandMap.insert(std::make_pair(r_433MHz->getCommandName(), std::move(r_433MHz)));

    std::unique_ptr <command> MPD(new command_mpd("MPD"));
    commandMap.insert(std::make_pair(MPD->getCommandName(), std::move (MPD)));

    std::unique_ptr <command> RS232 (new commandRS232("RS232"));
    commandMap.insert(std::make_pair(RS232->getCommandName(), std::move(RS232)));
}

std::string CommandHandlerMQTT::run(std::vector<std::string> &v, thread_data *my_data)
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
