#include "commandrs232.h"

commandRS232::commandRS232(const std::string &name):command(name)
{
}
std::string commandRS232::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "default";


    if (v[1] == "send")
    {
        my_data->main_Rs232->print(v[2]);
        str_buf = "done";
    }
    else if (v[1] == "error")
    {
        if (v.size() < 3 )
            return "add more parameter to error";
        std::string msg;
        for (unsigned int i = 2; i < v.size(); ++i)
        {
            msg.push_back(' ');
            msg.append(v[i]);
        }
     }
    else
    {
        str_buf = ("wrong parameter: " + v[1]);
    }
    return str_buf;
}

std::string commandRS232::help() const
{
    std::stringstream help;
    help << "RS232 - communication with Arduino:" << std::endl;
    help << "parameter:" << std::endl << std::endl;
    help << "\tsend <command> - send <command> to Arduino" << std::endl;

    return help.str();
}
