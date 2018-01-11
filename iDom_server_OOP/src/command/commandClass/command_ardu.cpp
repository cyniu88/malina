#include "command_ardu.h"

command_ardu::command_ardu(std::string name):command(name)
{

}

std::string command_ardu::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "only for internal usage!";
    if (v.size() > 2){
        std::string msg;
        for (unsigned int i = 2 ; i < v.size(); ++i){
            msg+=" ";
            msg+=v[i];
        }
       my_data->myEventHandler.run("433MHz")->addEvent("odebrano kod 433MHz "+msg);
    }
    return str_buf;
}

std::string command_ardu::help()
{
    return " only for internal usege\n";
}
