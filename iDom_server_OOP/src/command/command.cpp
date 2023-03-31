#include "command.h"

command::command(const std::string &commandName) : commandName(commandName)
{
}

command::~command()
{
}

std::string command::getCommandName()
{
    return commandName;
}
