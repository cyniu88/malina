#include "command_433mhz.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../433MHz/RFLink/rflinkhandler.h"

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
        else if (v[1] == "delete" && v.size() == 3)
        {
            if (my_data->main_REC->nameExist(v[2]) == false)
            {
                return "equipment "+ v[2] + " not exist ";
            }
            my_data->main_REC->deleteRadioEq(v[2]);
            str_buf = v[2] + " deleted";
        }
        else if (v[1] == "add" && v.size() > 3) //zmień tu
        {
            RADIO_EQ_CONFIG cfg;
           /* if(v[2] == "SWITCH" && v.size() == 12)
                cfg.set(v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11]); // zmień tu
            else*/ if(v[2] == "BUTTON" && v.size() >= 6)
                cfg.set(v[2],v[3],v[4],v[5],v[6]); // zmień tu
            else if(v[2] == "WEATHER" && v.size() >= 4)
                cfg.set(v[2],v[3],v[4]); // zmień tu
            else
            {
                if (v.size() != 12)
                    return "mising paramiter!";
                cfg.set(v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11]); // zmień tu
                //return v[2] + " " + v[3] + " " + v[4] + "add more paramiter or wrong type";
}
            /////////////////////////////////////////////////////////////
            if (my_data->main_REC->nameExist(v[3]) == true)
            {
                return "equipment "+ v[3] + " exist ";
            }
            try
            {
                my_data->main_REC->addRadioEq(cfg,v[2]);
            }
            catch(const WRONG_FORMAT& )
            {
                return "wrong type " + v[2];
            }
            catch(const std::invalid_argument& )
            {
                return "wrong ID " + v[4];
            }

            str_buf = v[2] + " " + v[3] + " added";
            my_data->main_REC->saveConfig(my_data->server_settings->_server.radio433MHzConfigFile);
        }
        else if (v[1] == "show" && v[2] == "switch"){
            str_buf = "";
            for (auto m_switch : my_data->main_REC->getSwitchPointerVector())
            {
                str_buf.append(stateToString(m_switch->getState()) );
            }
        }
        else if (v[1] == "show" && v[2] == "aether"){
            str_buf.clear();
            for(auto itr = my_data->main_RFLink->m_rflinkMAP.begin();
                itr != my_data->main_RFLink->m_rflinkMAP.end();
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
    help << ("433MHz delete <name> - dalete radio equipment") <<std::endl;
    help << ("433MHz add <type> <name> <ID> <onCode> <offCode> <on15sec> <sunrise> <sunset> <lock> <unlock> - add radio equipment") <<std::endl;
    help << ("433MHz switch <name> ON/OFF/15s - change switch state") <<std::endl;
    help << ("433MHz show all - list all equipment by name") <<std::endl;
    help << ("433MHz show aether - show aether devices by ID") <<std::endl;
    help << ("433MHz show config - show 433MHz devices config") <<std::endl;
    help << ("433MHz send <msg> - send command") <<std::endl;
    return help.str();
}
