#include "command_recuperator.h"

command_recuperator::command_recuperator(const std::string &name) : command(name)
{
}

std::string command_recuperator::execute(std::vector<std::string> &v, thread_context *context)
{
    std::stringstream ss;
    if (v.size() < 3)
    {
        ss << "not enough arguments";
    }
    else if (v.size() == 3 and v[1] == "set" and v[2] == "away")
    {
        context->m_recuperator->setAwayMode();
        return "set away mode ";
    }
    else if (v.size() == 5 and v[1] == "set" and v[2] == "away")
    {
        int time = std::stoi(v[3]);
        std::string unit = v[4];
        context->m_recuperator->setAwayModeTime(true, time, unit);
        ss << "set away mode time: " << std::to_string(time) << " unit: " << unit;
    }
    else if (v.size() == 5 and v[1] == "set" and v[2] == "boost")
    {
        int time = std::stoi(v[3]);
        std::string unit = v[4];
        context->m_recuperator->setBoostMode(time, unit);
        ss << "set away mode time: " << std::to_string(time) << " unit: " << unit;
    }
    else
    {
        ss << "wrong arguments";
    }
    return ss.str();
}

std::string command_recuperator::help() const
{
    std::stringstream ss;
    ss << "recuperator  set away <timeout> <unit> - set away mode  <timeout> = int  <unit> = ((minutes / minute / min / m) / (hours / hour / h) / (days / day / d)))  if empty  pernament away mode" << std::endl;
    ss << "recuperator  set away - set away mode  pernament" << std::endl;
    ss << "recuperator  set boost <timeout> <unit> - set boost mode  <timeout> = int  <unit> = ((minutes / minute / min / m) / (hours / hour / h) / (days / day / d)))" << std::endl;
    return ss.str();
}
