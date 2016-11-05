#ifndef COMMAND_MPD_H
#define COMMAND_MPD_H

#include "../command.h"

class command_mpd : public command
{
public:
    command_mpd(std::string name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data);
};

#endif // COMMAND_MPD_H
