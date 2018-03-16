#ifndef COMMND_ARDU_H
#define COMMND_ARDU_H
#include <iostream>
#include <string>
#include <vector>
#include "../command.h"

class command_ardu : public command
{
public:
    command_ardu(std::string name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMND_ARDU_H
