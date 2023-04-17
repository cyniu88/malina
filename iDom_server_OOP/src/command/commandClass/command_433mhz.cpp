#include "command_433mhz.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../433MHz/RFLink/rflinkhandler.h"
#include "../../iDomTools/idomtools_interface.h"

command_433MHz::command_433MHz(const std::string &name):command(name)
{
}

command_433MHz::~command_433MHz()
{
    // puts("command_433MHz::~command_433MHz()");
}

std::string command_433MHz::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf = "wrong paramiter\n" + help();
    if (v.size() > 2){
        //////////////////////////// switch
        if (v[1] == "show" && v[2] == "all"){
            str_buf = my_data->main_REC->listAllName();
        }
        else if (v[1] == "show" && v[2] == "switch"){
            str_buf.clear();
            for (auto m_switch : my_data->main_REC->getSwitchPointerVector())
            {
                str_buf.append(stateToString(m_switch->getState()) );
            }
        }
        else if (v[1] == "show" && v[2] == "aether"){
            str_buf.clear();
            for(auto itr = my_data->main_RFLink->m_rflinkMAP.begin();
                itr not_eq my_data->main_RFLink->m_rflinkMAP.end();
                itr++)
            {
                str_buf.append(itr->second.read());
                str_buf.push_back('\n');
            }
            str_buf.push_back('.');

        }
        else if (v[1] == "show" && v[2] == "config"){
            str_buf = my_data->main_REC->showConfig(my_data->server_settings->_server.radio433MHzConfigFile);
        }
        else if (v[1] == "send"){
            str_buf = "sended! counter " + std::to_string(++counter);
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
                else if(v[3] == "change"){
                    m_switch->on_Off();
                }
                else{
                    str_buf = "unknown paramiter: ";
                    str_buf.append(v[3]);
                }
            }
            catch (std::string& error){
                str_buf = error;
            }
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
        }
        /////////////////////////////////////////////
    }
    return str_buf;
}

std::string command_433MHz::help() const
{
    std::stringstream help;
    help << ("433MHz switch <name> ON/OFF/15s/change - change switch state") <<std::endl;
    help << ("433MHz show all - list all equipment by name") <<std::endl;
    help << ("433MHz show aether - show aether devices by ID") <<std::endl;
    help << ("433MHz show config - show 433MHz devices config") <<std::endl;
    help << ("433MHz send <msg> - send command") <<std::endl;
    return help.str();
}
