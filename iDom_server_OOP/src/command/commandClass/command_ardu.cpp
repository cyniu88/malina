#include "command_ardu.h"

command_ardu::command_ardu(std::string name):command(name)
{

}

std::string command_ardu::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = " only for internal usage!";
    if (v.size() > 1){
        if(v[1] == "433MHz"){
            my_data->myEventHandler.run("433MHz")->addEvent("RFLink: "+v[2]);
            try {
                my_data->main_RFLink->
                        rflinkMAP[my_data->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],
                        "ID")].counter();
                my_data->main_RFLink->
                        rflinkMAP[my_data->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],
                        "ID")].msg = v[2];
            }
            catch(std::string e){
                std::cout << "wyjatek w szukaniu: " << e <<std::endl;
            }

        }
    }
    return str_buf;
}

std::string command_ardu::help()
{
    return " only for internal usege\n";
}
