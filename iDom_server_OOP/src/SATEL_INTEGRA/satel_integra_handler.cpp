#include <iostream>
#include <time.h>
#include <string>
#include"../functions/functions.h"
#include "satel_integra_handler.h"
SATEL_INTEGRA_HANDLER::SATEL_INTEGRA_HANDLER(thread_data *myData)
{
    my_data = myData;
    m_integra32.connectIntegra(my_data->server_settings->_satel_integra.host,
                               my_data->server_settings->_satel_integra.port);
    m_integra32.setIntegraPin(my_data->server_settings->_satel_integra.pin);
}

SATEL_INTEGRA_HANDLER::~SATEL_INTEGRA_HANDLER()
{
    m_integra32.disconnectIntegra();
}

void SATEL_INTEGRA_HANDLER::checkSatel()
{
    if(m_integra32.connectionState() != STATE::CONNECTED)
        return;

    std::string dataOut = m_integra32.checkIntegraOut();
    int counter = 1;
    for(const auto& d : dataOut){
        auto bs = std::bitset<8>(d);
#ifdef BT_TEST
        std::cout << "BITY " << bs.to_string() << std::endl;
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

void SATEL_INTEGRA_HANDLER::run()
{
    while(useful_F::go_while){
        checkSatel();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}
