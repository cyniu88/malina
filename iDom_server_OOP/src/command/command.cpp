#include "command.h"

command::command(std::string commandName ) : commandName(commandName)
{

    std::cout << "konstruktor command"<<std::endl;
}

command::~command()
{
    std::cout << "destruktor command"<<std::endl;
}

std::string command::getCommandName()
{
    return commandName;
}
