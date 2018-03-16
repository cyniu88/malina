#include "command_ardu.h"
#include "../../functions/functions.h"
#include "../../433MHz/RFLink/rflinkhandler.h"
#include "../../RADIO_433_eq/radio_433_eq.h"

command_ardu::command_ardu(std::string name):command(name)
{
    //m_button433MHzVector = useful_F::myStaticData->main_REC->getButtonPointerVector();
    //m_mainRadioButton = static_cast<RADIO_BUTTON*>(useful_F::myStaticData->main_REC->getEqPointer("locker"));

    puts("END");
}

std::string command_ardu::execute(std::vector<std::string> &v, thread_data *my_data)
{
    //std::cout << "ID znalezione "<< my_data->main_RFLink->getArgumentValueFromRFLinkMSG(v[2], "ID")<<std::endl;
    std::string str_buf = " only for internal usage!";
    std::cout << " dadada: " << my_data->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],"ID") <<std::endl;
    if (v.size() > 1){

        if(v[1] == "433MHz")
        {

            try{
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
            my_data->myEventHandler.run("433MHz")->addEvent("RFLink "+v[2]);

           // std::cout << "w ardu jest:\n" << v[1]<<"\n" << v[2]<<"\n;"<<std::endl;
           if (m_mainRadioButton->getID() == my_data->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],"ID") )
            {
                my_data->main_iDomTools->button433mhzLockerPressed();
            }


        }
    }
    return str_buf;
}

std::string command_ardu::help()
{
    return " only for internal usege\n";
}
