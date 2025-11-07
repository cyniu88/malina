#include <sstream>
#include "command_show.h"
#include "../../../src/functions/functions.h"

command_show::command_show(const std::string &name):command(name)
{

}

std::string command_show::execute(std::vector<std::string> &v, thread_context *context)
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
            return context->m_threadPool->printThreadNames();
        }
        else if (v[1] == "iDom" && v[2] == "key")
        {
            return context->m_keyHandler->listKEY();
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
    help << "\tthread          - show all server thread pid " << std::endl;
    help << "\tiDom key        - show all access key" << std::endl;

    return help.str();
}
