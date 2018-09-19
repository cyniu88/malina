#include "command.h"

command::command(const std::string &commandName ) : commandName(commandName)
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
