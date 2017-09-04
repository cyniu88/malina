#include "commandhandler.h"
#include "commandClass/command_mpd.h"
#include "commandClass/commandrs232.h"
#include "commandClass/command_uptime.h"
#include "commandClass/command_big.h"
#include "commandClass/command_clock.h"
#include "commandClass/command_hello.h"
#include "commandClass/command_help.h"
#include "commandClass/command_ip.h"
#include "commandClass/command_put.h"
#include "commandClass/command_sleep.h"
#include "commandClass/command_ok.h"
#include "commandClass/command_show.h"
#include "commandClass/command_idom.h"
#include "commandClass/command_stop.h"
#include "commandClass/commandexit.h"
#include "commandClass/commandtest.h"
#include "commandClass/command_log.h"

commandHandler::commandHandler(thread_data * my_data)
{
    std::unique_ptr <command> test(new commandTEST("test") );
    commandMap.insert( std::make_pair(test->getCommandName(),std::move( test )) );

    std::unique_ptr <command> stop(new command_stop("stop") );
    commandMap.insert( std::make_pair(stop->getCommandName(),std::move( stop )) );

    std::unique_ptr <command> eexit(new commandEXIT("exit"));
    commandMap.insert( std::make_pair(eexit->getCommandName(),std::move( eexit )) );

    std::unique_ptr <command> MPD(new command_mpd("MPD"));
    commandMap.insert(std::make_pair(MPD->getCommandName(), std::move (MPD)));

    std::unique_ptr <command> RS232 (new commandRS232("RS232"));
    commandMap.insert(std::make_pair(RS232->getCommandName(), std::move(RS232)));

    std::unique_ptr <command> uptime (new command_UPTIME("uptime"));
    commandMap.insert(std::make_pair(uptime->getCommandName(), std::move(uptime)));

    std::unique_ptr <command> big (new command_big("big"));
    commandMap.insert(std::make_pair(big->getCommandName(), std::move(big)));

    std::unique_ptr <command> clock (new command_clock("clock"));
    commandMap.insert(std::make_pair(clock->getCommandName(), std::move(clock)));

    std::unique_ptr <command> hello (new command_hello("hello"));
    commandMap.insert(std::make_pair(hello->getCommandName(), std::move(hello)));

    std::unique_ptr <command> help (new command_help("help"));
    commandMap.insert(std::make_pair(help->getCommandName(), std::move(help)));

    std::unique_ptr <command> ip (new command_ip("ip"));
    commandMap.insert(std::make_pair(ip->getCommandName(), std::move(ip)));

    std::unique_ptr <command> ok (new command_ok("ok"));
    commandMap.insert(std::make_pair(ok->getCommandName(), std::move(ok)));

    std::unique_ptr <command> show (new command_show("show"));
    commandMap.insert(std::make_pair(show->getCommandName(), std::move(show)));

    std::unique_ptr <command> sleep (new command_sleep("sleep"));
    commandMap.insert(std::make_pair(sleep->getCommandName(), std::move(sleep)));

    std::unique_ptr <command> put (new command_put("put"));
    commandMap.insert(std::make_pair(put->getCommandName(), std::move(put)));

    std::unique_ptr <command> iDom (new command_iDom("iDom"));
    commandMap.insert(std::make_pair(iDom->getCommandName(), std::move(iDom)));

    std::unique_ptr <command> log (new command_log("log"));
    commandMap.insert(std::make_pair(log->getCommandName(), std::move(log)));

    this->my_data = my_data;
    this->my_data->commandMapPtr = &commandMap;
}

commandHandler::~commandHandler()
{
    puts("koniec destruktor command handler klasa bazowa");
}

std::string commandHandler::run(std::vector<std::string> &v, thread_data *my_data)
{
    if (commandMap.find(v[0]) == commandMap.end()){
        return "unknown command: "+ v[0];
    }
    else{
        return  commandMap[v[0]]->execute(v,my_data);
    }
}
