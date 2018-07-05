#include <iostream>
#include "../commandhandler.h"

std::string commandHandler::run(std::vector<std::string> &v, thread_data *my_data)
{
    std::cout << "commandHandler::run(std::vector<std::string> &v, thread_data *my_data) "<<std::endl;
    return "stub done";
}
