#ifndef COMMAND_IDOM_H
#define COMMAND_IDOM_H
#include <string>
#include <vector>

#include "../command.h"

class command_iDom : public command
{
public:
    command_iDom(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMAND_IDOM_H
