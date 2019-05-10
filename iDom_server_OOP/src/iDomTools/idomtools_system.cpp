#include "idomtools.h"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include "../functions/functions.h"
#include "../433MHz/RFLink/rflinkhandler.h"

std::string iDomTOOLS::getSystemInfo()
{
    double load[3];
    if (getloadavg(load, 3) != -1)
    {
#ifdef BT_TEST
        printf("load average : %f , %f , %f\n", load[0],load[1],load[2]);
#endif
    }
    const double megabyte = 1024 * 1024;
    struct sysinfo info;
    sysinfo(&info);

    long input_seconds = info.uptime;
    auto days = input_seconds / 60 / 60 / 24;
    auto hours = (input_seconds / 60 / 60) % 24;
    auto minutes = (input_seconds / 60) % 60;
    auto seconds = input_seconds % 60;

    std::stringstream ret;
    ret << "System uptime: " << days <<" day " << hours
        << " hours " << minutes << " minutes "
        << seconds << " seconds " << std::endl << "Load: "
        << (info.loads[0]/65536) << "% - 1 min, " <<(info.loads[1]/65536)
            << "% - 5 min, "<<(info.loads[2]/65536) << "% - 15 min." << std::endl
                                                    << "process count : " << info.procs << std::endl
                                                    << "total RAM   : "<< (info.totalram / megabyte)<< "MB" << std::endl
                                                    << "free RAM   : " << (info.freeram / megabyte) << "MB" << std::endl;

    return ret.str();
}

/////////////////////// health check //////////////////
void iDomTOOLS::healthCheck()
{
    int alarm_433MHz = 2131;

    if(my_data->mqttHandler->_connected == false){
        puts("brak polaczenia z serverm MQTT");
    }
    if(my_data->mqttHandler->_subscribed == false)
    {
        puts("brak subskrybcji do servera mqtt");
    }

    ////////////// RFLink ///////////
    auto t = Clock::getUnixTime()- my_data->main_RFLink->pingTime;

    if(t > 310 && my_data->mqttHandler->_subscribed == true)
    {
        m_restartAlarmRFLink++;
        if(m_restartAlarmRFLink == 2)
            my_data->main_iDomTools->reloadHard_iDomServer();

        std::string m("brak połaczenia RS232 z RFLink'iem");
        std::cout << "brak pingu RFLinka 433MHz t: " << t << std::endl;
        my_data->iDomAlarm.raiseAlarm(alarm_433MHz, m);
        my_data->main_iDomTools->sendViberMsg(m,my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                              my_data->server_settings->_fb_viber.viberSender + "health");
    }

    else{
        my_data->iDomAlarm.clearAlarm(alarm_433MHz);
        m_restartAlarmRFLink = 0;
    }

}

void iDomTOOLS::reloadSoft_iDomServer()
{
    std::string s ="close server";
    useful_F::send_to_arduino_clock(my_data, "RELO");
    iDomTOOLS::MPD_stop();
    my_data->iDomProgramState = iDomStateEnum::RELOAD;
    my_data->main_iDomTools->saveState_iDom();
    throw s;
}

void iDomTOOLS::reloadHard_iDomServer()
{
    std::string s ="close server";
    useful_F::send_to_arduino_clock(my_data, "UPDA");
    iDomTOOLS::MPD_stop();
    my_data->iDomProgramState = iDomStateEnum::HARD_RELOAD;
    my_data->main_iDomTools->saveState_iDom();
    throw s;
}

void  iDomTOOLS::close_iDomServer ()
{
    std::string s ="close server";
    useful_F::send_to_arduino_clock(my_data, "STOP");
    iDomTOOLS::MPD_stop();
    my_data->iDomProgramState = iDomStateEnum::CLOSE;
    my_data->main_iDomTools->saveState_iDom();
    throw s;
}
