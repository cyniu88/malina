#include "commandrs232.h"

#include "../iDom_server_OOP/src/functions/functions.h"

commandRS232::commandRS232(std::string name):command(name)
{

}

std::string commandRS232::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf ="";

        if (v[1]=="get")
        {
            if (v[2]=="temperature"){
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
        else
        {
            str_buf = ("wrong parameter: "+v[1]);
        }
        return str_buf;
}

std::string commandRS232::help()
{
    std::string help = "RS232 - communication with Arduino:";
                help.append("\n");
                help.append("parameter:");
                help.append("\n\n");
                help.append("\tsend <command> - send  <command>  to Arduino");
                help.append("\n");
                help.append("\tget <param> - get <param> from Arduino");
                help.append("\n");
                help.append("\t\ttemperature - get temeprature INSIDE + OUTSIDE");
                help.append("\n");

    return help;
}
