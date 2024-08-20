#include "rc_433mhz.h"
#include "RFLink/rflinkhandler.h"

RC_433MHz::RC_433MHz(thread_context *context)
{
    this->m_context = context;
}

void RC_433MHz::sendCode(const std::string &code)
{
    // std::string recv = useful_F::send_to_arduino(m_context,"433MHz:"+ code +";");
    m_context->main_RFLink->sendCommand(code);
}

std::string RC_433MHz::receiveCode()
{
    return m_context->main_RFLink->readFromRS232();
}
