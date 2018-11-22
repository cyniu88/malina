#include "commandrs232.h"

#include "/home/pi/programowanie/iDom_server_OOP/src/functions/functions.h"

commandRS232::commandRS232(const std::string &name):command(name)
{
}
std::string commandRS232::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf ="default";

    if (v[1]=="get")
    {
        if (v[2]=="temperature")
        {
            str_buf = useful_F::send_to_arduino(my_data,"temperature:339;");
            str_buf += std::to_string(++counter);
        }
        else
        {
            str_buf = ("wrong parameter: "+v[2]);
        }
    }

    else if (v[1]=="send")
    {
        str_buf = useful_F::send_to_arduino(my_data,v[2]);
    }
    else if (v[1]=="error")
    {
        std::string msg;
        for (unsigned int i = 2; i < v.size(); ++i)
        {
            msg+=" ";
            msg+=v[i];
        }
        if(v[2]=="Temperature" && v[3]=="error")
        {

            my_data->myEventHandler.run("RS232")->addEvent("RS232 error event: "+msg);
            log_file_mutex.mutex_lock();
            log_file_cout << WARNING<< "RS232 ERROR event: " << msg << std::endl;
            log_file_mutex.mutex_unlock();
        }
        else if (v[2]=="test" && v[3]=="msg")
        {
            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "RS232 ERROR debug : "<<msg << std::endl;
            log_file_mutex.mutex_unlock();
        }
        else
        {
            log_file_mutex.mutex_lock();
            log_file_cout << CRITICAL << "RS232 ERROR unknown : "<<msg << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }
    else
    {
        str_buf = ("wrong parameter: "+v[1]);
    }
    return str_buf;
}

std::string commandRS232::help()
{
    std::stringstream help;
    help << "RS232 - communication with Arduino:" << std::endl;
    help << "parameter:" << std::endl << std::endl;
    help << "\tsend <command> - send <command> to Arduino" << std::endl;
    help << "\tget <param> - get <param> from Arduino" << std::endl;
    help << "\t\ttemperature - get temeprature INSIDE + OUTSIDE" << std::endl;

    return help.str();
}
