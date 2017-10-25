#include "commandhandlerroot.h"
#include "commandClass/command_cmd.h"
#include "commandClass/command_event.h"

commandHandlerRoot::commandHandlerRoot(thread_data * my_data): commandHandler(my_data)
{
    //puts("konstruktor command handler root");
    std::unique_ptr <command> cmd (new command_cmd("cmd"));
    commandMap.insert(std::make_pair(cmd->getCommandName(), std::move(cmd)));

    std::unique_ptr <command> event (new command_event("event"));
    commandMap.insert(std::make_pair(event->getCommandName(), std::move(event)));

}

commandHandlerRoot::~commandHandlerRoot()
{
   // puts("koniec destruktor command handler root");
}
