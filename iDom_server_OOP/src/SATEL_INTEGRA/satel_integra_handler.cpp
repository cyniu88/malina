#include <iostream>
#include <time.h>
#include <string>
#include"../functions/functions.h"
#include "satel_integra_handler.h"
SATEL_INTEGRA_HANDLER::SATEL_INTEGRA_HANDLER(thread_data *myData): m_integra32(myData->server_settings->_satel_integra.host,
                                                                               myData->server_settings->_satel_integra.port)
{
    my_data = myData;
    m_integra32.connectIntegra(my_data->server_settings->_satel_integra.host,
                               my_data->server_settings->_satel_integra.port);
    m_integra32.setIntegraPin(my_data->server_settings->_satel_integra.pin);
    myData->satelIntegraHandler = static_cast<SATEL_INTEGRA_HANDLER_INTERFACE*>(this);

    checkAlarm(my_data->idom_all_state.alarmSatelState);
}

SATEL_INTEGRA_HANDLER::~SATEL_INTEGRA_HANDLER()
{
    m_integra32.disconnectIntegra();
}

void SATEL_INTEGRA_HANDLER::checkSatel()
{
    if(m_integra32.connectionState() not_eq STATE::CONNECTED)
        return;

    std::string dataOut = m_integra32.checkIntegraOut();
    int counter = 1;
    for(const auto& d : dataOut){
        auto bs = std::bitset<8>(d);
#ifdef BT_TEST
        //  std::cout << "BITY " << bs.to_string() << std::endl;
#endif
        for(int i = 0; i < 8; ++i){
            if(bs[i] == true){
#ifndef BT_TEST
                my_data->main_house_room_handler->satelSensorActive(counter);
#endif
            }
            ++counter;
        }
    }
}

void SATEL_INTEGRA_HANDLER::checkAlarm(STATE &st)
{
    if(m_integra32.connectionState() not_eq STATE::CONNECTED)
        return;
    bool fromSatel = m_integra32.isAlarmArmed();
    if((st == STATE::DISARMED or st == STATE::UNDEFINE) and fromSatel == true){
        st = STATE::ARMED;
        my_data->main_iDomTools->lockHome();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Alarm uzbrojony" << std::endl;
        log_file_mutex.mutex_unlock();
        my_data->main_iDomTools->sendViberPicture("alarm uzbrojony",
                                                  "https://frontpoint.files.wordpress.com/2010/08/redbutton_web_blog.png",
                                                  my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                  my_data->server_settings->_fb_viber.viberSender);
    }
    else if((st == STATE::ARMED or st == STATE::UNDEFINE) and fromSatel == false){
        st = STATE::DISARMED;
        my_data->main_iDomTools->unlockHome();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Alarm deaktywowany" << std::endl;
        log_file_mutex.mutex_unlock();
        my_data->main_iDomTools->sendViberPicture("alarm deaktywowany",
                                                  "https://www.clipartmax.com/png/middle/172-1721538_home-security-alarm-icons-lock-house-monitoring-system.png",
                                                  my_data->server_settings->_fb_viber.viberReceiver.at(0),
                                                  my_data->server_settings->_fb_viber.viberSender);
    }
    else if(st == STATE::UNDEFINE)
    {
        if(fromSatel == true)
            st = STATE::ARMED;
        else
            st = STATE::DISARMED;

        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "ustawiam stan alarmu na " << st << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void SATEL_INTEGRA_HANDLER::run()
{
    int sleepTime = 1000;
    while(useful_F::go_while){
        if(m_integra32.connectionState() not_eq STATE::CONNECTED)
        {
            m_integra32.connectIntegra(my_data->server_settings->_satel_integra.host,
                                       my_data->server_settings->_satel_integra.port);
            if(sleepTime < 60000)
                sleepTime += 100;
        }
        else{
            sleepTime = 1000;
        }
        checkAlarm(my_data->idom_all_state.alarmSatelState);
        checkSatel();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

SATEL_INTEGRA_INTERFACE *SATEL_INTEGRA_HANDLER::getSatelPTR()
{
#ifdef BT_TEST
    std::cout << "SATEL_INTEGRA_HANDLER::getSatelPTR()" << std::endl;
#endif
    return static_cast<SATEL_INTEGRA*>(&m_integra32);
}
