#include "command_log.h"

command_log::command_log(const std::string &name):command(name)
{
}

std::string command_log::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string msg;
    for (unsigned int i = 2; i < v.size(); ++i)
    {
        msg.push_back(' ');
        msg.append(v[i]);
    }
    log_file_mutex.mutex_lock();
    log_file_cout <<"USER:- " << logLevel(v[1]) << msg <<std::endl;;
    log_file_mutex.mutex_unlock();
    return "DONE!";
}

std::string command_log::help() const
{
    std::stringstream help;
    help << "log <logLevel> \"***TXT***\"- add *** to iDom logfile" << std::endl;
    return help.str();
}

logger_level command_log::logLevel(const std::string &level)
{
    if (level == "VERBOSE"){
        return VERBOSE;
    }
    else if (level == "DEBUG"){
        return DEBUG;
    }
    else if (level == "INFO"){
        return INFO;
    }
    else if (level == "WARNING"){
        return WARNING;
    }
    else if (level == "ERROR"){
        return ERROR;
    }
    else if (level == "FATAL"){
        return FATAL;
    }
    else if (level == "CRITICAL"){
        return CRITICAL;
    }
puts ("nie mam co wysetlic wale verbose");
        return VERBOSE;

}
