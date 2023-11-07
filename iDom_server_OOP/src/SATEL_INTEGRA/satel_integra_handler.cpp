#include <iostream>
#include <time.h>
#include <string>
#include <bitset>

#include "../functions/functions.h"
#include "satel_integra_handler.h"
#include "../iDomTools/idomtools_interface.h"

using namespace std::chrono_literals;

SATEL_INTEGRA_HANDLER::SATEL_INTEGRA_HANDLER(thread_data *myData) : m_integra32(myData->server_settings->_satel_integra.host,
                                                                                myData->server_settings->_satel_integra.port)
{
    context = myData;
    m_integra32.connectIntegra(context->server_settings->_satel_integra.host,
                               context->server_settings->_satel_integra.port);
    m_integra32.setIntegraPin(context->server_settings->_satel_integra.pin);
    myData->satelIntegraHandler = static_cast<SATEL_INTEGRA_HANDLER_INTERFACE *>(this);

    this->checkAlarm(context->idom_all_state.alarmSatelState);
}

SATEL_INTEGRA_HANDLER::~SATEL_INTEGRA_HANDLER()
{
    m_integra32.disconnectIntegra();
}

void SATEL_INTEGRA_HANDLER::checkSatel()
{
    if (m_integra32.connectionState() not_eq STATE::CONNECTED)
        return;

    std::string dataOut = m_integra32.checkIntegraOut();
    if (dataOut == "BAD INTEGRA DATA")
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "received bad integra data" << std::endl;
        log_file_mutex.mutex_unlock();
        return;
    }
    int counter = 1;
    for (const auto &d : dataOut)
    {
        auto bs = std::bitset<8>(d);
#ifdef BT_TEST
        //  std::cout << "BITY " << bs.to_string() << std::endl;
#endif
        for (int i = 0; i < 8; ++i)
        {
            if (bs[i] == true)
            {
#ifndef BT_TEST
                context->main_house_room_handler->satelSensorActive(counter);
#endif
            }
            ++counter;
        }
    }
}

void SATEL_INTEGRA_HANDLER::checkAlarm(STATE &st)
{
    if (m_integra32.connectionState() not_eq STATE::CONNECTED)
        return;
    bool fromSatel = m_integra32.isAlarmArmed();
    if ((st == STATE::DISARMED or st == STATE::UNDEFINE) and fromSatel == true)
    {
        st = STATE::ARMED;
        context->main_iDomTools->lockHome();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Alarm uzbrojony" << std::endl;
        log_file_mutex.mutex_unlock();
        context->main_iDomTools->sendViberPicture("alarm uzbrojony",
                                                  "https://frontpoint.files.wordpress.com/2010/08/redbutton_web_blog.png",
                                                  context->server_settings->_fb_viber.viberReceiver.at(0),
                                                  context->server_settings->_fb_viber.viberSender);
    }
    else if ((st == STATE::ARMED or st == STATE::UNDEFINE) and fromSatel == false)
    {
        st = STATE::DISARMED;
        context->main_iDomTools->unlockHome();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Alarm deaktywowany" << std::endl;
        log_file_mutex.mutex_unlock();
        context->main_iDomTools->sendViberPicture("alarm deaktywowany",
                                                  "https://www.clipartmax.com/png/middle/172-1721538_home-security-alarm-icons-lock-house-monitoring-system.png",
                                                  context->server_settings->_fb_viber.viberReceiver.at(0),
                                                  context->server_settings->_fb_viber.viberSender);
    }
    else if (st == STATE::UNDEFINE)
    {
        if (fromSatel == true)
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
    auto sleepTime = 1000ms;
    while (useful_F::go_while)
    {
        try
        {
            if (m_integra32.connectionState() not_eq STATE::CONNECTED)
            {
                m_integra32.connectIntegra(context->server_settings->_satel_integra.host,
                                           context->server_settings->_satel_integra.port);
                if (sleepTime < 60s)
                    sleepTime += 100ms;
            }
            else
            {
                sleepTime = 1s;
            }
            checkAlarm(context->idom_all_state.alarmSatelState);
            checkSatel();
            std::this_thread::sleep_for(sleepTime);
        }
        catch (...)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << ERROR << "wyjątek w  SATEL_INTEGRA_HANDLER::run()" << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }
}

SATEL_INTEGRA_INTERFACE *SATEL_INTEGRA_HANDLER::getSatelPTR()
{
#ifdef BT_TEST
    std::cout << "SATEL_INTEGRA_HANDLER::getSatelPTR()" << std::endl;
#endif
    return static_cast<SATEL_INTEGRA *>(&m_integra32);
}
