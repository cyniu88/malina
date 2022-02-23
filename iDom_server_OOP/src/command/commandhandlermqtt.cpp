#include "commandhandlermqtt.h"
#include "../../libs/emoji/emoji.h"
#include "commandClass/command_433mhz.h"
#include "commandClass/commandrs232.h"
#include "commandClass/command_mpd.h"
#include "commandClass/command_event.h"
#include "commandClass/command_log.h"
#include "commandClass/command_wifi.h"
#include "commandClass/command_buderus.h"
#include "commandClass/command_light.h"
#include "commandClass/command_idom.h"
#include "commandClass/command_wylacz.h"
#include "commandClass/command_wlacz.h"

CommandHandlerMQTT::CommandHandlerMQTT()
{
    std::unique_ptr <command> r_433MHz (new command_433MHz("433mhz"));
    commandMap.insert(std::make_pair(r_433MHz->getCommandName(), std::move(r_433MHz)));

    std::unique_ptr <command> MPD(new command_mpd("mpd"));
    commandMap.insert(std::make_pair(MPD->getCommandName(), std::move (MPD)));

    std::unique_ptr <command> RS232 (new commandRS232("rs232"));
    commandMap.insert(std::make_pair(RS232->getCommandName(), std::move(RS232)));

    std::unique_ptr <command> log (new command_log("log"));
    commandMap.insert(std::make_pair(log->getCommandName(), std::move(log)));

    std::unique_ptr <command> event (new command_event("event"));
    commandMap.insert(std::make_pair(event->getCommandName(), std::move(event)));

    std::unique_ptr <command> wifi (new command_wifi("wifi"));
    commandMap.insert(std::make_pair(wifi->getCommandName(), std::move(wifi)));

    std::unique_ptr <command> buderus (new command_buderus("buderus"));
    commandMap.insert(std::make_pair(buderus->getCommandName(), std::move(buderus)));

    std::unique_ptr <command> light (new command_light("light"));
    commandMap.insert(std::make_pair(light->getCommandName(), std::move(light)));

    std::unique_ptr <command> idom (new command_iDom("idom"));
    commandMap.insert(std::make_pair(idom->getCommandName(), std::move(idom)));

    std::unique_ptr <command> wlacz (new command_iDom("włącz"));
    commandMap.insert(std::make_pair(wlacz->getCommandName(), std::move(wlacz)));

    std::unique_ptr <command> wylacz (new command_iDom("wyłącz"));
    commandMap.insert(std::make_pair(wylacz->getCommandName(), std::move(wylacz)));
}

std::string CommandHandlerMQTT::run(std::vector<std::string> &v, thread_data *my_data)
{
    //
   // for(auto &vv : v)
    {
        for(auto &z : v[0]){
            if(z >= 'A' && z <= 'Z') //gdy napotkamy małą literę zamieniamy ją na dużą
                z += 32; //przesuwając o 32 miejsca w kodzie ASCII
        }
    }

    if (commandMap.find(v[0]) == commandMap.end()){
        std::fstream log;
        log.open( "/mnt/ramdisk/command.txt", std::ios::binary | std::ios::in | std::ios::out|std::ios::app );
        log << "MQTT: " << v[0] << std::endl;
        log.close();
        return EMOJI::emoji(E_emoji::WARNING_SIGN) + " unknown command: "+ v[0];
    }
    else{
        return commandMap[v[0]]->execute(v,my_data);
    }
}
