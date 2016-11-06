#include "commandhandler.h"
#include "commandClass/command_mpd.h"
#include "commandClass/commandrs232.h"

commandHandler::commandHandler()
{
    std::unique_ptr <command> test(new commandTEST("TEST") );
    commandMap.insert( std::make_pair(test->getCommandName(),std::move( test )) );

    std::unique_ptr <command> exit(new commandEXIT("EXIT"));
    commandMap.insert( std::make_pair(exit->getCommandName(),std::move( exit )) );

    std::unique_ptr <command> MPD(new command_mpd("MPD"));
    commandMap.insert(std::make_pair(MPD->getCommandName(), std::move (MPD)));

    std::unique_ptr <command> RS232 (new commandRS232("RS232"));
    commandMap.insert(std::make_pair(RS232->getCommandName(), std::move(RS232)));
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
