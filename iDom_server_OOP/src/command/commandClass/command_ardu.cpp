#include "command_ardu.h"
#include "../../idom_exceptions.h"

command_ardu::command_ardu(const std::string &name):command(name)
{
    this->m_mainRadioButton = nullptr;
    this->m_mainWeatherStation = nullptr;
}

command_ardu::command_ardu(const std::string& name,
                           thread_context *context):command(name),
    m_button433MHzVector(context->main_REC->getButtonPointerVector()),
    m_mainRadioButton(nullptr),
    m_weatherStVe(context->main_REC->getWeather_StationPtrVector())
{
    try{
            m_mainWeatherStation = static_cast<RADIO_WEATHER_STATION*>(context->main_REC->getEqPointer("first"));
    }
    catch(const NOT_FOUND_ERROR &e)
    {
        context->iDomAlarm.raiseAlarm(88756, e.what());
    }
}

std::string command_ardu::execute(std::vector<std::string> &v, thread_context *context)
{
    std::string str_buf = " only for internal usage!";
    if (v.size() > 1){
        if(v[1] == "show"){
            if (m_mainWeatherStation != nullptr){
                str_buf = m_mainWeatherStation->data.getDataString();
            } else {
                str_buf = "weather station not configured";
            }
        }
        if(v[1] == "433MHz"){
            if (v.size() <= 2) return str_buf;
            context->myEventHandler.run("433MHz")->addEvent("RFLink: " + v[2]);
            handle433MHzData(context, v[2]);
        }
    }
    return str_buf;
}

void command_ardu::handle433MHzData(thread_context *context, const std::string &msg)
{
    try {
        context->main_RFLink->
            m_rflinkMAP[context->main_RFLink->getArgumentValueFromRFLinkMSG(msg, "ID")].counter();
        context->main_RFLink->
            m_rflinkMAP[context->main_RFLink->getArgumentValueFromRFLinkMSG(msg, "ID")].msg = msg;
    }
    catch(const WRONG_FORMAT&){
        throw; // re-rzucamy dalej — złapie wywołujący (test lub handler wyżej)
    }
    catch(const NOT_FOUND_ERROR&){
        pingAndOkRecv(context, msg);
    }

    try {
        for(auto k : m_button433MHzVector)
        {
            if (k->getID() == context->main_RFLink->getArgumentValueFromRFLinkMSG(msg, "ID"))
            {
                context->mqttHandler->publish("iDom-client/command", k->getCommandRun());
            }
        }
    }
    catch (const NOT_FOUND_ERROR&) { /* ID not present in this message — not an error */ }

    try {
        if (m_mainWeatherStation != nullptr &&
            m_mainWeatherStation->getID() ==
                context->main_RFLink->getArgumentValueFromRFLinkMSG(msg, "ID"))
        {
            m_mainWeatherStation->data.putData(msg);
        }
    }
    catch (const NOT_FOUND_ERROR&) { /* ID not present in this message — not an error */ }
}

std::string command_ardu::help() const
{
    return " only for internal usege\n";
}

void command_ardu::pingAndOkRecv(thread_context *context, const std::string& s)
{
    if (s.find("OK;") not_eq std::string::npos)
        context->main_RFLink->m_okTime = Clock::getUnixTime();
    else if (s.find("PONG;") not_eq std::string::npos)
        context->main_RFLink->m_pingTime = Clock::getUnixTime();
}
