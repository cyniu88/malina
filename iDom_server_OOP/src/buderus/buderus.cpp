#include "buderus.h"

#include "../functions/functions.h"

BUDERUS::BUDERUS()
{
    m_className.append(typeid(this).name());
    iDom_API::addToMap(m_className, this);
#ifdef BT_TEST
    std::cout << "BUDERUS::BUDERUS()" << std::endl;
#endif
}

BUDERUS::~BUDERUS()
{
    iDom_API::removeFromMap(m_className);
}

void BUDERUS::updateBoilerDataFromMQTT(nlohmann::json jj)
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    try
    {
        if(jj.contains("outdoorTemp")){
            m_outdoorTemp = jj.at("outdoorTemp").get<double>();
            m_boiler_data = jj;
        }
        if(jj.contains("wWStorageTemp2")){
            m_boilerTemp = jj.at("wWStorageTemp2").get<double>();
            m_boiler_data_ww = jj;
        }
        if(jj.contains("curFlowTemp"))
            m_curFlowTemp = jj.at("curFlowTemp").get<double>();
        if(jj.contains("burnGas")){
            auto burnGas = jj.at("burnGas").get<std::string>();
            if (burnGas == "on" && m_heating_active == false)
            {
                m_heating_active = true;
                m_heating_time = Clock::getUnixTime();
            }
            else if (burnGas == "off" && m_heating_active == true)
            {
                m_heating_active = false;
            }
        }
        if(jj.contains("wWCirc")){
            if (jj.at("wWCirc").get<std::string>() == "on")
            {
                if (m_circlePump not_eq STATE::ON)
                {
                    useful_F::myStaticData->main_iDomTools->sendViberMsg("uruchamiam pompe obiegową CWU",
                                                                         useful_F::myStaticData->server_settings->_fb_viber.viberReceiver.at(0),
                                                                         useful_F::myStaticData->server_settings->_fb_viber.viberSender + "BUDERUS");
                    useful_F::myStaticData->main_Rs232->print("LED_ALARM:1");
                    m_circlePump = STATE::ON;
                }
            }
            else
            {
                if (m_circlePump == STATE::ON)
                {
                    useful_F::myStaticData->main_iDomTools->sendViberMsg("zakończono precę pompy obiegowej CWU",
                                                                         useful_F::myStaticData->server_settings->_fb_viber.viberReceiver.at(0),
                                                                         useful_F::myStaticData->server_settings->_fb_viber.viberSender + "BUDERUS");
                    useful_F::myStaticData->main_Rs232->print("LED_ALARM:0");
                    m_circlePump = STATE::OFF;
                }
            }
        }
    }
    catch (nlohmann::json::exception &e)
    {
        std::cout << "wyjatek w boiler data " << e.what() << std::endl;
    }
}

void BUDERUS::updateThermostatDataFromMQTT(nlohmann::json jj)
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    m_thermostat_data = jj;
    m_insideTemp = jj["hc1"].at("currtemp").get<double>();
}

void BUDERUS::tapWaterActivated()
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    m_tapwater_active = true;
}

void BUDERUS::tapWaterDeactivated()
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    m_tapwater_active = false;
}

void BUDERUS::heatingActivated()
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    m_heating_active = true;
}

void BUDERUS::heatingDeactivated()
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    m_heating_active = false;
}

void BUDERUS::setHeating(bool b)
{
    m_heating_active = b;
}

void BUDERUS::setTapWater(bool b)
{
    m_tapwater_active = b;
}

bool BUDERUS::isHeatingActiv()
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    return m_heating_active;
}

bool BUDERUS::isTapWaterActiv()
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    return m_tapwater_active;
}

std::string BUDERUS::getAllData() const
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    std::stringstream ret;
    ret << dump();

    return ret.str();
}

double BUDERUS::getOutdoorTemp() const
{
    return m_outdoorTemp;
}

double BUDERUS::getInsideTemp() const
{
    return m_insideTemp;
}

double BUDERUS::getBoilerTemp() const
{
    return m_boilerTemp;
}

double BUDERUS::getCurFlowTemp() const
{
    return m_curFlowTemp;
}

void BUDERUS::circlePompToRun()
{
    if (m_boilerTemp > 55 && m_heating_active == true && m_circlePompCanRun == true)
    {
        runCirclePompForWhile();
        m_circlePompCanRun = false;
    }

    if (m_boilerTemp < 60 && m_heating_active == false && m_circlePompCanRun == false)
    {
        m_circlePompCanRun = true;
    }
}

void BUDERUS::runCirclePompForWhile()
{
    useful_F::myStaticData->mqttHandler->publish("iDom-client/buderus/ems-esp/boiler",
                                                 R"({"cmd":"wwcirculation","data":"on"})");
}

STATE BUDERUS::getCirclePumpState()
{
    return m_circlePump;
}

void BUDERUS::boilerHeatOneTime()
{
    useful_F::myStaticData->mqttHandler->publish("iDom-client/buderus/ems-esp/boiler",
                                                 R"({"cmd":"wwonetime","data":"on"})");
}

void BUDERUS::setTempInsideBuilding(const std::string &t)
{
    std::stringstream tt;
    tt << R"({"cmd":"temp","data":)";
    tt << t;
    tt << R"(,"id":1})";
    useful_F::myStaticData->mqttHandler->publish("iDom-client/buderus/ems-esp/thermostat", tt.str());
}

unsigned int BUDERUS::getHeatingStartTime() const
{
    std::lock_guard<std::mutex> lock(m_lockGuard);
    return m_heating_time;
}

std::string BUDERUS::getDump()
{
    return dump();
}

std::string BUDERUS::dump() const
{
    std::stringstream ret;

    ret << '{' << std::endl;
    ret << R"("m_boiler_data": )" << m_boiler_data.dump(4) << "," << std::endl;
    ret << R"("m_boiler_data_ww": )" << m_boiler_data_ww.dump(4) << "," << std::endl;
    ret << R"("m_thermostat_data": )" << m_thermostat_data.dump(4) << "," << std::endl;
    ret << R"("m_tapwater_active": )" << m_tapwater_active << "," << std::endl;
    ret << R"("m_heating_active": )" << m_heating_active << "," << std::endl;
    ret << R"("m_heating_time":)" << m_heating_time << ',' << std::endl;
    ret << R"("m_boilerTemp": )" << m_boilerTemp << "," << std::endl;
    ret << R"("m_insideTemp": )" << m_insideTemp << "," << std::endl;
    ret << R"("m_curFlowTemp": )" << m_curFlowTemp << "," << std::endl;
    ret << R"("m_outdoorTemp": )" << m_outdoorTemp << "," << std::endl;
    ret << R"("m_circlePompCanRun": )" << m_circlePompCanRun << "," << std::endl;
    ret << R"("m_circlePump": ")" << m_circlePump << R"(")" << std::endl
        << "}" << std::endl;

    return ret.str();
}
