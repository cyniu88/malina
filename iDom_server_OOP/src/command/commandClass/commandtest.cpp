#include "commandtest.h"
#include "../../functions/functions.h"
#include "../../../ftplibpp/ftplib.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{
   useful_F::downloadFile(my_data->server_settings->cameraURL,
                                          "/home/pi/plik.jpg",
                                          10);

   ftplib myFTP;

   myFTP.Connect(my_data->server_settings->ftpServer.URL.c_str());
   myFTP.Login(my_data->server_settings->ftpServer.user.c_str(),my_data->server_settings->ftpServer.pass.c_str());

   myFTP.Put("/home/pi/plik.jpg","/twardziel/iDom/mojFTPplik.jpg", ftplib::image);

   myFTP.Quit();


    std::string msg="test - for test ";
    std::string txt = my_data->main_iDomTools->getTextToSpeach();
    if (v.size() >1){
    msg += my_data->main_iDomTools->postOnFacebook(v[1]+" \n"+ txt);
    msg += " \nviber: ";
    msg += my_data->main_iDomTools->sendViberPicture(v[1],
            "http://cyniu88.no-ip.pl/images/iDom/iDom/mojFTPplik.jpg",
            my_data->server_settings->viberReceiver.at(0),
            my_data->server_settings->viberSender);
    }



    return msg;
}

std::string commandTEST::help()
{
    return "test - for test\n";
}
