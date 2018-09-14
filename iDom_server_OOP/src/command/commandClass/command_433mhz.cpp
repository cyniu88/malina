#include "command_433mhz.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../433MHz/RFLink/rflinkhandler.h"

command_433MHz::command_433MHz(std::string name):command(name)
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
                return "equipment "+ v[2]+" not exist ";
            }
            my_data->main_REC->deleteRadioEq(v[2]);
            str_buf = v[2] + " deleted";
        }
        else if (v[1] == "add" && v.size() == 12)  //zmień tu
        {
            RADIO_EQ_CONFIG cfg;
            cfg.set(v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11]); // zmień tu
            if (my_data->main_REC->nameExist(v[2]) == true)
            {
                return "equipment "+ v[2]+" exist ";
            }
            try
            {
            my_data->main_REC->addRadioEq(cfg,v[4]);
            }
            catch(WRONG_FORMAT)
            {
                return "wrong type "+v[4];
            }
            catch(std::invalid_argument)
            {
                return "wrong ID "+v[3];
            }

            str_buf = v[2] + " added";
            my_data->main_REC->saveConfig(my_data->server_settings->radio433MHzConfigFile);
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
            str_buf += ".";

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
            my_data->main_iDomTools->saveState_iDom();
        }
        /////////////////////////////////////////////
    }
    return str_buf;
}

std::string command_433MHz::help()
{
    std::stringstream help;
    help << ("433MHz delete <name> - dalete radio equipment") <<std::endl;
    help << ("433MHz add <name> <ID> <type> <onCode> <offCode> <on15sec> <sunrise> <sunset> <lock> <unlock> - add radio equipment") <<std::endl;
    help << ("433MHz switch <name> ON/OFF/15s - change switch state") <<std::endl;
    help << ("433MHz show all - list all equipment by name") <<std::endl;
    help << ("433MHz show aether   - show aether devices by ID") <<std::endl;
    help << ("433MHz send <msg>    - send command") <<std::endl;
    return help.str();
}
