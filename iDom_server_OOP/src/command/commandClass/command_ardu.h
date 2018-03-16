#ifndef COMMND_ARDU_H
#define COMMND_ARDU_H
#include <iostream>
#include <string>
#include <vector>
#include "../command.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../433MHz/RFLink/rflinkhandler.h"

class command_ardu : public command
{
public:
    command_ardu(std::string name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
    std::string help();
};

#endif // COMMND_ARDU_H
