#include <sstream>
#include "command_show.h"
#include "../../../src/functions/functions.h"

command_show::command_show(const std::string &name):command(name)
{

}

std::string command_show::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "show what?";
    if (v.size() > 1){
        if (v[1] == "log")
        {
            if (v.size() >2 && v.size() < 4){
                 return useful_F::l_send_file(_logfile,v[2],true);
            }
            else if (v.size() > 3){
                if (v[2] == "no"){
                 return useful_F::l_send_file(_logfile,v[3],false);
                }
                else{
                    return "do you mean : show log no <string>?";
                }
            }
            return useful_F::l_send_file(_logfile,"");
        }
        if (v[1] == "thread")
        {
            if (v.size() < 3){
                return "No ID";
            }
            else {
                if (v [2] not_eq"all"){
                    std::stringstream ss;
                    ss << my_data->main_THREAD_arr->at(std::stoi(v[2])).thread_ID;
                    str_buf = my_data->main_THREAD_arr->at(std::stoi(v[2])).thread_name;
                    str_buf.append("ID: ");
                    str_buf.append(ss.str());
                    str_buf.append(" socket: ");
                    str_buf.append(std::to_string(my_data->main_THREAD_arr->at(std::stoi(v[2])).thread_socket));
                    return str_buf;
                }
                else{
                    str_buf.erase();
                    std::stringstream ss;
                    for (unsigned int i = 0; i < iDomConst::MAX_CONNECTION;++i)
                    {
                        ss.clear();
                        ss = std::stringstream();
                        str_buf.append(std::to_string(i) + " ");
                        str_buf.append(my_data->main_THREAD_arr->at(i).thread_name);
                        str_buf.append("\t ID: ");
                        ss << my_data->main_THREAD_arr->at(i).thread_ID;
                        str_buf.append(ss.str());
                        unsigned int idSocket = my_data->main_THREAD_arr->at(i).thread_socket;

                        if (idSocket not_eq 0 && idSocket not_eq 1){
                            str_buf.append(" socket: ");
                            str_buf.append(std::to_string(my_data->main_THREAD_arr->at(i).thread_socket));
                        }
                        str_buf.push_back('\n');
                    }
                    return str_buf;
                }
            }
        }
        else if (v[1] == "iDom" && v[2] == "key")
        {
            return my_data->m_keyHandler->listKEY();
        }
        else {
            return "wrong parameter: " + v[1];
        }
    }
    return str_buf;
}

std::string command_show::help() const
{
    std::stringstream help;
    help << "show <parameter> - for show something" <<std::endl << std::endl;
    help << "parameter:" << std::endl;
    help << "\tlog             - show all server log" << std::endl;
    help << "\tlog <string>    - show all server log lines which contain <string>" << std::endl;
    help << "\tlog no <string> - show all server log lines which NO contain <string>" << std::endl;
    help << "\tthread all      - show all server thread pid " << std::endl;
    help << "\tthread <number> - show server <number> thread pid " << std::endl;
    help << "\tiDom key        - show all access key" << std::endl;

    return help.str();
}
