#ifndef COMMAND_IDOM_H
#define COMMAND_IDOM_H
#include <string>
#include <vector>

#include "../command.h"

class command_iDom : public command
{
public:
    explicit command_iDom(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_IDOM_H
