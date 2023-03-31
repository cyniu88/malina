#include "rc_433mhz.h"
#include "RFLink/rflinkhandler.h"

RC_433MHz::RC_433MHz(thread_data *my_data)
{
    this->m_my_data = my_data;
}

void RC_433MHz::sendCode(const std::string &code)
{
    // std::string recv = useful_F::send_to_arduino(m_my_data,"433MHz:"+ code +";");
    m_my_data->main_RFLink->sendCommand(code);
}

std::string RC_433MHz::receiveCode()
{
    return m_my_data->main_RFLink->readFromRS232();
}
