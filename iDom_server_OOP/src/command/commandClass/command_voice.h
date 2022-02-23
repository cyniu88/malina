#ifndef COMMAND_VOICE_H
#define COMMAND_VOICE_H

#include "../command.h"

class command_voice: public command
{
public:
    explicit command_voice(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * my_data) override;
    std::string help() const override;
};

#endif // COMMAND_VOICE_H