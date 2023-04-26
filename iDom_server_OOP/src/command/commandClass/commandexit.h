#ifndef COMMANDEXIT_H
#define COMMANDEXIT_H

#include "../command.h"

class commandEXIT : public command
{
public:
    explicit commandEXIT(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMANDEXIT_H
