#include "command_ardu.h"

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
    catch(const std::exception &e)
    {
        context->iDomAlarm.raiseAlarm(88756, e.what());
    }
}

std::string command_ardu::execute(std::vector<std::string> &v, thread_context *context)
{
    std::string str_buf = " only for internal usage!";
    if (v.size() > 1){
        if(v[1] == "show"){
            RADIO_WEATHER_STATION* st = static_cast<RADIO_WEATHER_STATION*>(context->main_REC->getEqPointer("first"));
            str_buf = st->data.getDataString();
        }
        if(v[1] == "433MHz"){
            context->myEventHandler.run("433MHz")->addEvent("RFLink: " + v[2]);
            try {
                context->main_RFLink->
                    m_rflinkMAP[context->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],
                                                                                    "ID")].counter();
                context->main_RFLink->
                    m_rflinkMAP[context->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],
                                                                                    "ID")].msg = v[2];
            }
            catch(const WRONG_FORMAT&){
                throw; // re-rzucamy dalej — złapie wywołujący (test lub handler wyżej)
            }
            catch(const std::exception& e){
                //std::cout << "wyjatek w szukaniu: " << e.what() <<std::endl;
                pingAndOkRecv( context, v[2]);
            }
            //TODO add command
            try {
                for(auto k : m_button433MHzVector)
                {
                    if (k->getID() == context->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],"ID") )
                    {
                        context->mqttHandler->publish("iDom-client/command", k->getCommandRun() );
                    }
                }
            }
            catch (const std::exception& e){ }
            try {
                if (m_mainWeatherStation->getID() ==
                        context->main_RFLink->getArgumentValueFromRFLinkMSG(v[2],"ID") )
                {
                    m_mainWeatherStation->data.putData(v[2]);
                }
            }
            catch (const std::exception& e){ }
        }
    }
    return str_buf;
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
