#include "command.h"

command::command(std::string commandName ) : commandName(commandName)
{
    std::cout << "konstruktor command"<<std::endl;
}

command::~command()
{
    //puts("destruktor command");
}

std::string command::getCommandName()
{
    return commandName;
}
