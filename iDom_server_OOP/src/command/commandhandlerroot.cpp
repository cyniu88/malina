#include "commandhandlerroot.h"
#include "commandClass/command_cmd.h"
#include "commandClass/command_event.h"
#include "commandClass/command_433mhz.h"
#include "commandClass/command_ardu.h"

commandHandlerRoot::commandHandlerRoot(thread_data * my_data): commandHandler(my_data)
{
    std::unique_ptr <command> cmd (new command_cmd("cmd"));
    commandMap.insert(std::make_pair(cmd->getCommandName(), std::move(cmd)));

    std::unique_ptr <command> event (new command_event("event"));
    commandMap.insert(std::make_pair(event->getCommandName(), std::move(event)));

    std::unique_ptr <command> r_433MHz (new command_433MHz("433MHz"));
    commandMap.insert(std::make_pair(r_433MHz->getCommandName(), std::move(r_433MHz)));

    std::unique_ptr <command> ardu (new command_ardu("ardu"));
    commandMap.insert(std::make_pair(ardu->getCommandName(), std::move(ardu)));
}

commandHandlerRoot::~commandHandlerRoot()
{
   // puts("koniec destruktor command handler root");
}
