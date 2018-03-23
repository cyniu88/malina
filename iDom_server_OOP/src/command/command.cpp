#include "command.h"

command::command(std::string commandName ) : commandName(commandName)
{
   // std::cout << "konstruktor command"<<std::endl;
}

command::~command()
{
  // puts("command::~command()");
}

std::string command::getCommandName()
{
    return commandName;
}
