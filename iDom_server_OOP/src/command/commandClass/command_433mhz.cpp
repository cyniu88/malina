#include "command_433mhz.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../433MHz/RFLink/rflinkhandler.h"

command_433MHz::command_433MHz(std::string name):command(name)
{
}

std::string command_433MHz::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "command 433MHz - wrong paramiter: ";
    if (v.size() > 2){
        //////////////////////////// switch
        if (v[1] == "show" && v[2] == "all"){
            str_buf = my_data->main_REC->listAllName();
        }
        else if (v[1] == "show" && v[2] == "switch"){
            str_buf = "testowo";
            for (auto m_switch : my_data->main_REC->getSwitchPointerVector()){
                m_switch->getState();
            }
        }
        else if (v[1] == "show" && v[2] == "aether"){
            str_buf.clear();
            for(auto itr = my_data->main_RFLink->rflinkMAP.begin();
                itr != my_data->main_RFLink->rflinkMAP.end();
                itr++)
            {
                str_buf += itr->second.read();
                str_buf += '\n';
            }
        }
        else if (v[1] == "send" ){
            str_buf = "sended!;";
            my_data->main_RFLink->sendCommand(v[2]);

        }
        else if (v[1] == "switch"){
            try{
                RADIO_SWITCH *m_switch = dynamic_cast<RADIO_SWITCH*>(my_data->main_REC->getEqPointer(v[2]));

                if (v[3] == "ON") {
                    m_switch->on();
                    str_buf = " done ";
                }
                else if (v[3] == "OFF"){
                    m_switch->off();
                    str_buf = " done ";
                }
                else if (v[3] == "15s"){
                    m_switch->onFor15sec();
                    str_buf = " done ";
                }
                else{
                    str_buf = "unknown paramiter: ";
                    str_buf.append(v[3]);
                }
            }
            catch (std::string error){
                str_buf = error;
            }
        }
        /////////////////////////////////////////////
    }
    return str_buf;
}

std::string command_433MHz::help()
{
    std::string ret;
    ret.append("433MHz show all - list all equipment by name\n");
    ret.append("433MHz switch <name> ON/OFF/15s - change switch state\n");
    ret.append("433MHz show aether   - show aether devices by ID\n");
    ret.append("433MHz send <msg>    - send command\n");
    return ret;
}
