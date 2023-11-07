#ifndef COMMAND_VOICE_H
#define COMMAND_VOICE_H

#include "../command.h"

class command_voice: public command
{
public:
    explicit command_voice(const std::string& name);
    std::string execute(std::vector <std::string> &v,thread_data * context) override;
    std::string help() const override;
//private:
    bool vectorContain(const std::vector<std::string>& vector, const std::string& str);
};

#endif // COMMAND_VOICE_H
