#include "command_show.h"
#include "../iDom_server_OOP/src/functions/functions.h"

command_show::command_show(std::string name):command(name)
{

}

std::string command_show::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "show what? ";
    if (v.size() > 1){
        if (v[1] =="log")
        {
            if (v.size() >2){
                 return l_send_file(_logfile,v[2],true);
            }
            else if (v.size() > 3){
                 return l_send_file(_logfile,v[3],false);
            }
            else{
            return l_send_file(_logfile,"");
            }
        }
        if (v[1]=="thread")
        {
            if (v.size() < 3){
                return " No ID";
            }
            else {
                if (v [2] !="all"){
                    str_buf  = my_data->main_THREAD_arr[std::stoi(v[2])].thread_name;
                    str_buf  += " ID: ";
                    str_buf  += std::to_string(my_data->main_THREAD_arr[std::stoi(v[2])].thread_ID);
                    str_buf  += " socket: ";
                    str_buf  += std::to_string(my_data->main_THREAD_arr[std::stoi(v[2])].thread_socket);
                    return str_buf;
                }
                else{
                    str_buf.erase();
                    for (int i =0 ; i< MAX_CONNECTION;++i)
                    {
                        str_buf  += std::to_string(i)+"\t";
                        str_buf  += my_data->main_THREAD_arr[i].thread_name;
                        str_buf  += "\t ID: ";
                        str_buf  += std::to_string(my_data->main_THREAD_arr[i].thread_ID);

                        if (my_data->main_THREAD_arr[i].thread_socket !=0){
                            str_buf  += " socket: ";
                            str_buf  += std::to_string(my_data->main_THREAD_arr[i].thread_socket);
                        }
                        str_buf  += "\n";
                    }
                    return str_buf;
                }
            }

        }
        else {
            return "wrong parameter: "+v[1];
        }
    }


    return str_buf;
}

std::string command_show::help()
{
    std::string help = "show <parameter>- for show something";
    help.append("\n\n");
    help.append("parameter:");
    help.append("\n");
    help.append("\tlog             - show all server log");
    help.append("\n");
    help.append("\tlog <string>    - show all server log lines which contain <string>");
    help.append("\n");
    help.append("\tlog no <string> - show all server log lines which NO contain <string>");
    help.append("\n");
    help.append("\tthread all      - show all server thread pid ");
    help.append("\n");
    help.append("\tthread <number> - show server <number> thread pid ");
    help.append("\n");

    return help;
}

