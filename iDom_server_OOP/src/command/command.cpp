#include "command.h"

command::command(const std::string &commandName) : commandName(commandName)
{
}

std::string command::getCommandName() const
{
    return commandName;
}
