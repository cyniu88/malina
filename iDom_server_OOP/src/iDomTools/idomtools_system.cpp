#include "idomtools.h"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include "../functions/functions.h"
#include "../433MHz/RFLink/rflinkhandler.h"
#include "../thread_functions/iDom_thread.h"

std::string iDomTOOLS::getSystemInfo()
{
    double load[3];
    if (getloadavg(load, 3) not_eq -1)
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
    auto t = Clock::getUnixTime()- my_data->main_RFLink->m_pingTime;

    if(t > 310 && my_data->mqttHandler->_subscribed == true)
    {
        m_restartAlarmRFLink++;
        if(m_restartAlarmRFLink == 2){
            log_file_mutex.mutex_lock();
            log_file_cout << WARNING << "restart servera z powodu braku polaczenia z RFLinkiem" << std::endl;
            log_file_mutex.mutex_unlock();
            my_data->main_iDomTools->reloadHard_iDomServer();
        }
        //else if(m_restartAlarmRFLink == 2){
            my_data->main_RFLink = std::make_shared<RFLinkHandler>(my_data);
            useful_F::sleep(1);
            my_data->main_RFLink->init();

            //my_data->main_RFLink->sendCommand("10;REBOOT;");
        //}
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

std::string iDomTOOLS::dump() const
{
    std::stringstream ret;

    ret << " m_restartAlarmRFLink: " << this->m_restartAlarmRFLink << std::endl;
    ret << " m_lightningStruct.data: " << this->m_lightningStruct.data.str() << std::endl;
    ret << " m_lightningStruct.distance: " << this->m_lightningStruct.distance << std::endl;
    ret << " m_lightningStruct.riseAlarm: " << this->m_lightningStruct.riseAlarm << std::endl;
    ret << " m_lightningStruct.timestamp: " << this->m_lightningStruct.timestamp << std::endl;
    ret << " key: " << this->m_key << std::endl;
    ret << " lastButton433MHzLockUnlockTime: " << this->m_lastButton433MHzLockUnlockTime.getString() << std::endl;
    ret << " sendLightingCounter: " << this->sendLightingCounter << std::endl;
    return ret.str();
}

std::string iDomTOOLS::openGateLink(std::vector<std::string> v)
{
    generator gg;
    std::string tempName = gg.random_string(20);
    m_keyHandler->addTempKEY(tempName, my_data->server_settings->_gateway.keySize);
    std::string key = m_keyHandler->getKEY(tempName);
    std::stringstream ret;

    ret << my_data->server_settings->_gateway.url << tempName << "&" << key ;

    for(const auto& s : v){
        ret << "&" << s;
    }

    return ret.str();
}

void iDomTOOLS::reloadSoft_iDomServer()
{
    std::string s = "close server";
    my_data->iDomProgramState = iDomStateEnum::RELOAD;
    my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
    throw s;
}

void iDomTOOLS::reloadHard_iDomServer()
{
    std::string s = "close server";
    my_data->iDomProgramState = iDomStateEnum::HARD_RELOAD;
    my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
    throw s;
}

void  iDomTOOLS::close_iDomServer ()
{
    std::string s = "close server";
    iDomTOOLS::MPD_stop();
    my_data->iDomProgramState = iDomStateEnum::CLOSE;
    my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
    throw s;
}
