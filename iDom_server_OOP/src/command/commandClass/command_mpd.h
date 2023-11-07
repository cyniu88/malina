#ifndef COMMAND_MPD_H
#define COMMAND_MPD_H

#include "../command.h"

class command_mpd : public command
{
public:
    explicit command_mpd(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
};

#endif // COMMAND_MPD_H
