#include <iostream>
#include "../../libs/emoji/emoji.h"
#include "commandhandler.h"
#include "commandClass/command_uptime.h"
#include "commandClass/command_big.h"
#include "commandClass/command_clock.h"
#include "commandClass/command_hello.h"
#include "commandClass/command_help.h"
#include "commandClass/command_ip.h"
#include "commandClass/command_put.h"
#include "commandClass/command_ok.h"
#include "commandClass/command_show.h"
#include "commandClass/commandtest.h"
#include "commandClass/command_log.h"
#include "commandClass/commandexit.h"
#include "commandClass/command_shed.h"

commandHandler::commandHandler(thread_data * my_data)
{
    std::unique_ptr <command> test(new commandTEST("test") );
    commandMap.insert( std::make_pair(test->getCommandName(),std::move( test )) );

    std::unique_ptr <command> uptime (new command_UPTIME("uptime"));
    commandMap.insert(std::make_pair(uptime->getCommandName(), std::move(uptime)));

    std::unique_ptr <command> big (new command_big("big"));
    commandMap.insert(std::make_pair(big->getCommandName(), std::move(big)));

    std::unique_ptr <command> clock (new command_clock("clock"));
    commandMap.insert(std::make_pair(clock->getCommandName(), std::move(clock)));

    std::unique_ptr <command> hello (new command_hello("hello"));
    commandMap.insert(std::make_pair(hello->getCommandName(), std::move(hello)));

    std::unique_ptr <command> help (new command_help("help", &commandMap));
    commandMap.insert(std::make_pair(help->getCommandName(), std::move(help)));

    std::unique_ptr <command> ip (new command_ip("ip"));
    commandMap.insert(std::make_pair(ip->getCommandName(), std::move(ip)));

    std::unique_ptr <command> ok (new command_ok("ok"));
    commandMap.insert(std::make_pair(ok->getCommandName(), std::move(ok)));

    std::unique_ptr <command> show (new command_show("show"));
    commandMap.insert(std::make_pair(show->getCommandName(), std::move(show)));

    std::unique_ptr <command> put (new command_put("put"));
    commandMap.insert(std::make_pair(put->getCommandName(), std::move(put)));

    std::unique_ptr <command> log (new command_log("log"));
    commandMap.insert(std::make_pair(log->getCommandName(), std::move(log)));

    std::unique_ptr <command> eexit(new commandEXIT("exit"));
    commandMap.insert( std::make_pair(eexit->getCommandName(),std::move( eexit )) );

    std::unique_ptr <command> shed(new command_shed("shed"));
    commandMap.insert(std::make_pair(shed->getCommandName(), std::move(shed)));

    this->my_data = my_data;
}

std::string commandHandler::run(std::vector<std::string> &v, thread_data *my_data)
{
    if (commandMap.find(v.front()) == commandMap.end()){
        std::fstream log;
        log.open( "/mnt/ramdisk/command.txt", std::ios::binary | std::ios::in | std::ios::out|std::ios::app );
        log << v.front() << std::endl;
        log.close();
        return EMOJI::emoji(E_emoji::WARNING_SIGN) + " unknown command: "+ v.front();
    }
    else{
        return commandMap[v.front()]->execute(v,my_data);
    }
}
