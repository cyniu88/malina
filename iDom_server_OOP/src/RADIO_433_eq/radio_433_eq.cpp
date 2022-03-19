#include <iostream>
#include <fstream>
#include <sstream>

#include "radio_433_eq.h"

RADIO_SWITCH::RADIO_SWITCH(thread_data *my_data,
                           const RADIO_EQ_CONFIG &cfg,
                           RADIO_EQ_TYPE type):
                                                 main433MHz(my_data),
                                                 m_sunrise(stringToState(cfg.sunrise)),
                                                 m_sunset(stringToState(cfg.sunset))
{
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
    RADIO_EQ::m_config = cfg;
}

RADIO_SWITCH::~RADIO_SWITCH()
{
}

void RADIO_SWITCH::on()
{
    if(RADIO_EQ::m_config.onCode not_eq "null")
    {
        main433MHz.sendCode(RADIO_EQ::m_config.onCode);
        m_state = STATE::ON;
        RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name, STATE::ON);
    }
    else {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << RADIO_EQ::m_config.name << " switch - zla konfiguracja kodu ON" << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void RADIO_SWITCH::off()
{
    if(RADIO_EQ::m_config.offCode not_eq "null")
    {
        main433MHz.sendCode(RADIO_EQ::m_config.offCode);
        m_state = STATE::OFF;
        RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name,STATE::OFF);
    }
    else {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << RADIO_EQ::m_config.name << " switch - zla konfiguracja kodu OFF" << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void RADIO_SWITCH::on_Off()
{
    if(m_state not_eq STATE::OFF){
        main433MHz.sendCode(RADIO_EQ::m_config.offCode);
        m_state = STATE::OFF;
        RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name,STATE::OFF);
        return;
    }
    if(m_state not_eq STATE::ON){
        main433MHz.sendCode(RADIO_EQ::m_config.onCode);
        m_state = STATE::ON;
        RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name,STATE::ON);
        return;
    }
}

void RADIO_SWITCH::onFor15sec()
{
    if(RADIO_EQ::m_config.on15sec not_eq "null")
    {
        main433MHz.sendCode(RADIO_EQ::m_config.on15sec);
        m_state = STATE::TEMPORARY;
        RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name,STATE::TEMPORARY);
    }
    else {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << RADIO_EQ::m_config.name <<
            " switch - zla konfiguracja kodu ON for 15s" << std::endl;
        log_file_mutex.mutex_unlock();
    }
}

void RADIO_SWITCH::onSunrise()
{
    if(m_sunrise == STATE::ON ){
        on();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " ON due to sunrise");
    }
    else if(m_sunrise == STATE::OFF){
        off();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " OFF due to sunrise");
    }
}

void RADIO_SWITCH::onSunset()
{
    if(m_sunset == STATE::ON ){
        on();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " ON due to sunset");
    }
    else if(m_sunset == STATE::OFF){
        off();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " OFF due to sunset");
    }
}

void RADIO_SWITCH::onLockHome()
{
    if (m_config.lock == "ON")
    {
        on();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " ON due to 433MHz button pressed");
    }
    else if(m_config.lock == "OFF")
    {
        off();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " OFF due to 433MHz button pressed");
    }
}

void RADIO_SWITCH::onUnlockHome()
{
    if (m_config.unlock == "ON")
    {
        on();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch " +
                                                        RADIO_EQ::m_config.name +
                                                        " ON due to 433MHz button pressed");
    }
    else if (m_config.unlock == "OFF")
    {
        off();
        m_my_data->myEventHandler.run("iDom")->addEvent("radio switch "
                                                        + RADIO_EQ::m_config.name +
                                                        " OFF due to 433MHz button pressed");
    }
}

STATE RADIO_SWITCH::getState() const
{
    return m_state;
}

std::string RADIO_SWITCH::getName() const
{
    return RADIO_EQ::m_config.name;
}

std::string RADIO_SWITCH::getID() const
{
    return RADIO_EQ::m_config.ID;
}

void RADIO_SWITCH::setCode(RADIO_EQ_CONFIG cfg)
{
    RADIO_EQ::m_config.onCode = cfg.onCode;
    RADIO_EQ::m_config.offCode = cfg.offCode;
    RADIO_EQ::m_config.on15sec = cfg.on15sec;
    RADIO_EQ::m_config.type = cfg.type;
    RADIO_EQ::m_config.lock = cfg.lock;
    RADIO_EQ::m_config.unlock = cfg.unlock;
    RADIO_EQ::m_config.name = cfg.name;

    if(cfg.sunset == "on"){
        m_sunset = STATE::ON;
    }
    if(cfg.sunset == "off"){
        m_sunset = STATE::OFF;
    }
    if(cfg.sunrise == "on"){
        m_sunrise = STATE::ON;
    }
    if(cfg.sunrise == "off"){
        m_sunrise = STATE::OFF;
    }
}

RADIO_EQ_CONTAINER::RADIO_EQ_CONTAINER(thread_data *my_data)
{
    this->my_data = my_data;
}

RADIO_EQ_CONTAINER::~RADIO_EQ_CONTAINER()
{
    for(auto it = m_radioEqMap.begin(); it not_eq m_radioEqMap.end(); ++it) {
        delete it->second;
    }
}

void RADIO_EQ_CONTAINER::addRadioEq( RADIO_EQ_CONFIG cfg, RADIO_EQ_TYPE type)
{
    switch (type) {
    case RADIO_EQ_TYPE::SWITCH:
        m_radioEqMap.insert(std::make_pair(cfg.name, new RADIO_SWITCH(my_data, cfg, RADIO_EQ_TYPE::SWITCH) ) );
        break;
    case RADIO_EQ_TYPE::BUTTON:
        m_radioEqMap.insert(std::make_pair(cfg.name, new RADIO_BUTTON(my_data, cfg, RADIO_EQ_TYPE::BUTTON) ) );
        break;
    case RADIO_EQ_TYPE::WEATHER_S:
        m_radioEqMap.insert(std::make_pair(cfg.name,
                                           new RADIO_WEATHER_STATION(my_data, cfg, RADIO_EQ_TYPE::WEATHER_S) ) );
        break;
    default:
        break;
    }
}

void RADIO_EQ_CONTAINER::addRadioEq(RADIO_EQ_CONFIG cfg, const std::string& type)
{
    RADIO_EQ_TYPE ret = RADIO_EQ_TYPE::NONE;
    if(type == "SWITCH") ret = RADIO_EQ_TYPE::SWITCH;
    else if(type == "BUTTON") ret = RADIO_EQ_TYPE::BUTTON;
    else if(type == "WEATHER") ret = RADIO_EQ_TYPE::WEATHER_S;
    else if(type == "PIR") ret = RADIO_EQ_TYPE::PIR;
    else if(type == "GATE") ret = RADIO_EQ_TYPE::GATE;
    else {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL <<" throw RADIO_EQ_CONTAINER::addRadioEq()" << std::endl;
        log_file_mutex.mutex_unlock();
        throw WRONG_FORMAT();
    }
    std::cout << " id ma : " << cfg.ID << std::endl;
    (void)std::stoi(cfg.ID); //check ID is number
    addRadioEq(cfg,ret);
}

void RADIO_EQ_CONTAINER::deleteRadioEq(const std::string& name)
{
    delete m_radioEqMap[name];
    m_radioEqMap.erase(name);
    saveConfig(my_data->server_settings->_server.radio433MHzConfigFile);
}

RADIO_EQ* RADIO_EQ_CONTAINER::getEqPointer(std::string name)
{
    auto m = m_radioEqMap.find(name);
    if (m not_eq m_radioEqMap.end()){
        return (m->second);
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL <<" throw RADIO_EQ_CONTAINER::getEqPointer()"<<std::endl;
        log_file_mutex.mutex_unlock();
        throw std::string("433MHz equipment not found "+name);
    }
}

std::vector<RADIO_SWITCH*> RADIO_EQ_CONTAINER::getSwitchPointerVector()
{
    std::vector<RADIO_SWITCH*> switchVector;
    for (const auto& it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::SWITCH){
            switchVector.push_back(static_cast<RADIO_SWITCH*>(it.second));
        }
    }
    return switchVector;
}

std::vector<RADIO_BUTTON *> RADIO_EQ_CONTAINER::getButtonPointerVector()
{
    std::vector<RADIO_BUTTON*> buttonVector;
    for (const auto& it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::BUTTON){
            buttonVector.push_back(static_cast<RADIO_BUTTON*>(it.second));
        }
    }
    return buttonVector;
}

std::vector<RADIO_WEATHER_STATION *> RADIO_EQ_CONTAINER::getWeather_StationPtrVector()
{
    std::vector<RADIO_WEATHER_STATION*> weatherStVe;
    for (const auto& it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::WEATHER_S){
            weatherStVe.push_back(static_cast<RADIO_WEATHER_STATION*>(it.second));
        }
    }
    return weatherStVe;
}

std::string RADIO_EQ_CONTAINER::listAllName()
{
    std::stringstream allName;

    for(auto it = m_radioEqMap.begin(); it not_eq m_radioEqMap.end(); ++it) {
        allName << it->first  << "\t ID: " << it->second->getID() << "\t state: " << it->second->getState() << std::endl;
    }

    return allName.str();
}

bool RADIO_EQ_CONTAINER::nameExist(const std::string& name)
{
    bool exist = false;
    if(m_radioEqMap.find(name) not_eq m_radioEqMap.end())
    {
        exist = true;
    }
    return exist;
}

void RADIO_EQ_CONTAINER::loadConfig(const std::string& filePath)
{
    std::ifstream myfile (filePath);
    if (myfile.is_open())
    {
        m_radioEqMap.clear();
        nlohmann::json j;
        myfile >> j;
        RADIO_EQ_CONFIG cfg;

        try
        {
            nlohmann::json switchJson = j.at("SWITCH");
            for (nlohmann::json::iterator it = switchJson.begin(); it not_eq switchJson.end(); ++it)
            {
                nlohmann::json switchJson_tmp = it.value();
                cfg.name = switchJson_tmp.at("name").get<std::string>();
                cfg.ID  = switchJson_tmp.at("id").get<std::string>();
                cfg.offCode = switchJson_tmp.at("OFF").get<std::string>();
                cfg.onCode = switchJson_tmp.at("ON").get<std::string>();
                cfg.on15sec = switchJson_tmp.at("on15sec").get<std::string>();
                cfg.sunrise = switchJson_tmp.at("sunrise").get<std::string>();
                cfg.sunset = switchJson_tmp.at("sunset").get<std::string>();
                cfg.lock   = switchJson_tmp.at("lock").get<std::string>();
                cfg.unlock = switchJson_tmp.at("unlock").get<std::string>();
                cfg.type   = switchJson_tmp.at("type").get<std::string>();
                addRadioEq(cfg,RADIO_EQ_TYPE::SWITCH);
                dynamic_cast<RADIO_SWITCH*>(getEqPointer(cfg.name))->setCode(cfg);
            }
        }
        catch(...)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "no SWITCH equipment in config" << std::endl;
            log_file_mutex.mutex_unlock();
        }
        try
        {
            nlohmann::json buttonJson = j.at("BUTTON");
            for (nlohmann::json::iterator it = buttonJson.begin(); it not_eq buttonJson.end(); ++it)
            {
                nlohmann::json buttonJson_tmp = it.value();
                cfg.name = buttonJson_tmp.at("name").get<std::string>();
                cfg.ID  = buttonJson_tmp.at("id").get<std::string>();
                cfg.offCode = buttonJson_tmp.at("OFF").get<std::string>();
                cfg.onCode = buttonJson_tmp.at("ON").get<std::string>();
                cfg.type   = buttonJson_tmp.at("type").get<std::string>();
                cfg.commandRun = buttonJson_tmp.at("commandRun").get<std::string>();
                addRadioEq(cfg,RADIO_EQ_TYPE::BUTTON);
            }
        }
        catch(...)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "no BUTTONs equipment in config" << std::endl;
            log_file_mutex.mutex_unlock();
#ifdef BT_TEST
            std::cout << "no BUTTONs equipment in config" <<std::endl;
#endif
        }

        try
        {
            nlohmann::json weatherJson= j.at("WEATHER");
            for (nlohmann::json::iterator it = weatherJson.begin(); it not_eq weatherJson.end(); ++it)
            {
                nlohmann::json weatherJson_tmp = it.value();
                cfg.name = weatherJson_tmp.at("name").get<std::string>();
                cfg.ID  = weatherJson_tmp.at("id").get<std::string>();
                cfg.type = weatherJson_tmp.at("type").get<std::string>();
                addRadioEq(cfg,RADIO_EQ_TYPE::WEATHER_S);
            }
        }
        catch(...)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "no WEATHER STAIONs equipment in config" << std::endl;
            log_file_mutex.mutex_unlock();
#ifdef BT_TEST
            std::cout << "no WEATHER STAIONs equipment in config" <<std::endl;
#endif
        }

        myfile.close();
    }
    else std::cout << "Unable to open file";
}

void RADIO_EQ_CONTAINER::saveConfig(const std::string& filePath)
{
    nlohmann::json switchJson;
    nlohmann::json buttonJson;
    nlohmann::json weatherJson;

    std::vector<RADIO_SWITCH*> vSwitch = getSwitchPointerVector();
    for(auto s : vSwitch)
    {
        switchJson[s->getName()] = s->m_config.getJson();
    }

    std::vector<RADIO_BUTTON*> vButton = getButtonPointerVector();
    for (auto s : vButton)
    {
        buttonJson[s->getName()] = s->m_config.getJson();
    }

    std::vector<RADIO_WEATHER_STATION *> vWeather = getWeather_StationPtrVector();
    for(auto s : vWeather)
    {
        weatherJson[s->getName()] = s->m_config.getJson();
    }

    m_configJson["SWITCH"] = switchJson;
    m_configJson["BUTTON"] = buttonJson;
    m_configJson["WEATHER"] = weatherJson;
    // write prettified JSON to another file
    std::ofstream o(filePath);
    o << std::setw(4) << m_configJson << std::endl;
}

std::string RADIO_EQ_CONTAINER::showConfig(const std::string &filePath)
{
    std::string ret = "can not open file ";
    ret.append(filePath);

    std::ifstream myfile (filePath);
    if (myfile.is_open())
    {
        nlohmann::json j;
        myfile >> j;
        ret = j.dump(4);
    }
    return ret;
}

RADIO_EQ::RADIO_EQ(): m_type(RADIO_EQ_TYPE::NONE)
{
    this->m_my_data = std::nullptr_t();
}

RADIO_EQ::~RADIO_EQ()
{
}

RADIO_EQ_TYPE RADIO_EQ::getType() const
{
    return m_type;
}

RADIO_WEATHER_STATION::RADIO_WEATHER_STATION(thread_data *my_data, const RADIO_EQ_CONFIG& cfg, RADIO_EQ_TYPE type)
{
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
    RADIO_EQ::m_config = cfg;
}

RADIO_WEATHER_STATION::~RADIO_WEATHER_STATION()
{
}

STATE RADIO_WEATHER_STATION::getState() const
{
    return m_state;
}

std::string RADIO_WEATHER_STATION::getName() const
{
    return RADIO_EQ::m_config.name;
}

std::string RADIO_WEATHER_STATION::getID() const
{
    return RADIO_EQ::m_config.ID;
}

std::string WEATHER_STRUCT::dump() const
{
    std::stringstream ret;

    ret << "m_counter: " << this->m_counter << std::endl;
    ret << "m_humidity: " << this->m_humidity << std::endl;
    ret << "m_temperature: " << this->m_temperature << std::endl;
    ret << "m_barometricPressure: " << this->m_barometricPressure << std::endl;
    ret << "m_map_iDom_API.size() : " << this->m_map_iDom_API.size() << std::endl;

    return ret.str();
}
