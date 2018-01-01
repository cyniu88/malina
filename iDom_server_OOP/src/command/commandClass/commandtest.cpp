#include "commandtest.h"
#include "../../functions/functions.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{
   useful_F::downloadFile("https://stonehorsemongolia.com/wp-content/uploads/2017/04/Meditation-Retreat-Gobi-Desert-Mongolia-Gobi-Gurvan-Saikhan-National-Park.jpg",
                                          "/home/pi/plik.jpg",
                                          10);

    std::string msg="test - for test ";
    std::string txt = my_data->main_iDomTools->getTextToSpeach();
    if (v.size() >1){
    msg += my_data->main_iDomTools->postOnFacebook(v[1]+" \n"+ txt);
    msg += " \nviber: ";
    msg += my_data->main_iDomTools->sendViberPicture(v[1],
            "http://canacopegdl.com/images/cold/cold-14.jpg",
            my_data->server_settings->viberReceiver.at(0),
            my_data->server_settings->viberSender);
    }
    return msg;
}

std::string commandTEST::help()
{
    return "test - for test\n";
}
