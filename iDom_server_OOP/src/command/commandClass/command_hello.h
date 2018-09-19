#ifndef COMMAND_HELLO_H
#define COMMAND_HELLO_H

#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_hello : public command
{
public:
    command_hello(const std::string& name);
    ~command_hello();
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_HELLO_H
