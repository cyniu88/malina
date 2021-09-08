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

void SATEL_INTEGRA_HANDLER::checkAlarm(STATE& st)
{
    bool fromSatel = m_integra32.isAlarmArmed();
    if(st != STATE::ARMED and fromSatel == true){
        st = STATE::ARMED;
        my_data->main_iDomTools->lockHome();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Lock house due to arm satel alarm" << std::endl;
        log_file_mutex.mutex_unlock();
    }
    else if( st != STATE::DISARMED and fromSatel == false){
        st = STATE::DISARMED;
        my_data->main_iDomTools->unlockHome();
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "Unlock house due to disarm satel alarm" << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void SATEL_INTEGRA_HANDLER::run()
{
    while(useful_F::go_while){
        checkSatel();
        checkAlarm(my_data->idom_all_state.alarmSatelState);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}

SATEL_INTEGRA_INTERFACE *SATEL_INTEGRA_HANDLER::getSatelPTR()
{
#ifdef BT_TEST
    std::cout << "SATEL_INTEGRA_HANDLER::getSatelPTR()" << std::endl;
#endif
    return &m_integra32;
}
