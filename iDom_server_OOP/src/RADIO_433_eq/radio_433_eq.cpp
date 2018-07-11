#include <iostream>
#include <fstream>
#include <sstream>
#include "radio_433_eq.h"

RADIO_SWITCH::RADIO_SWITCH(thread_data *my_data, RADIO_EQ_CONFIG cfg, RADIO_EQ_TYPE type):
    main433MHz(my_data)
{
    puts("RADIO_SWITCH::RADIO_SWITCH()");
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
    RADIO_EQ::m_config = cfg;
}

RADIO_SWITCH::~RADIO_SWITCH()
{
    puts("RADIO_SWITCH::~RADIO_SWITCH()");
}

void RADIO_SWITCH::on()
{
    main433MHz.sendCode(RADIO_EQ::m_config.onCode);
    m_state = STATE::ON;
    RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name, STATE::ON);
}

void RADIO_SWITCH::off()
{
    main433MHz.sendCode(RADIO_EQ::m_config.offCode);
    m_state = STATE::OFF;
    RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(RADIO_EQ::m_config.name,STATE::OFF);
}

void RADIO_SWITCH::onFor15sec()
{
    main433MHz.sendCode(RADIO_EQ::m_config.on15sec);
    m_state = STATE::WORKING;
}

void RADIO_SWITCH::onSunrise()
{
    if(m_sunrise == STATE::ON ){
        on();
        m_my_data->myEventHandler.run("433MHz")->addEvent("radio switch "+RADIO_EQ::m_config.name + " ON due to sunrise");
    }
    else if(m_sunrise == STATE::OFF){
        off();
        m_my_data->myEventHandler.run("433MHz")->addEvent("radio switch "+RADIO_EQ::m_config.name + " OFF due to sunrise");
    }
}

void RADIO_SWITCH::onSunset()
{
    if(m_sunset == STATE::ON ){
        on();
        m_my_data->myEventHandler.run("433MHz")->addEvent("radio switch "+RADIO_EQ::m_config.name + " ON due to sunset");
    }
    else if(m_sunset == STATE::OFF){
        off();
        m_my_data->myEventHandler.run("433MHz")->addEvent("radio switch "+RADIO_EQ::m_config.name + " OFF due to sunset");
    }
}

STATE RADIO_SWITCH::getState()
{
    return m_state;
}

std::string RADIO_SWITCH::getName()
{
    return RADIO_EQ::m_config.name;
}

std::string RADIO_SWITCH::getID()
{
    return RADIO_EQ::m_config.ID;
}

void RADIO_SWITCH::setCode(RADIO_EQ_CONFIG cfg)
{
    RADIO_EQ::m_config.onCode = cfg.onCode;
    RADIO_EQ::m_config.offCode = cfg.offCode;
    RADIO_EQ::m_config.on15sec = cfg.on15sec;
    RADIO_EQ::m_config.type = cfg.type;

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
    puts("RADIO_EQ_CONTAINER::RADIO_EQ_CONTAINER()");
    this->my_data = my_data;
}

RADIO_EQ_CONTAINER::~RADIO_EQ_CONTAINER()
{
    for(auto it = m_radioEqMap.begin(); it != m_radioEqMap.end(); ++it) {
        delete it->second;
    }
    puts("RADIO_EQ_CONTAINER::~RADIO_EQ_CONTAINER()");
}

void RADIO_EQ_CONTAINER::addRadioEq( RADIO_EQ_CONFIG cfg, RADIO_EQ_TYPE type)
{
    switch (type) {
    case RADIO_EQ_TYPE::SWITCH:
        m_radioEqMap.insert(std::make_pair(cfg.name, new RADIO_SWITCH(my_data, cfg, RADIO_EQ_TYPE::SWITCH)  )    );
        break;
    case RADIO_EQ_TYPE::BUTTON:
        m_radioEqMap.insert(std::make_pair(cfg.name, new RADIO_BUTTON(my_data, cfg, RADIO_EQ_TYPE::BUTTON)  )    );
        break;
    case RADIO_EQ_TYPE::WEATHER_S:
        m_radioEqMap.insert(std::make_pair(cfg.name, new RADIO_WEATHER_STATION(my_data, cfg, RADIO_EQ_TYPE::WEATHER_S) ) );
        break;
    default:
        break;
    }
}

void RADIO_EQ_CONTAINER::addRadioEq(RADIO_EQ_CONFIG cfg, std::string type)
{
    RADIO_EQ_TYPE ret = RADIO_EQ_TYPE::NONE;
    if(type == "SWITCH") ret = RADIO_EQ_TYPE::SWITCH;
    else if(type == "BUTTON") ret = RADIO_EQ_TYPE::BUTTON;
    else if(type == "WEATHER") ret = RADIO_EQ_TYPE::WEATHER_S;
    else if(type == "PIR") ret = RADIO_EQ_TYPE::PIR;
    else if(type == "GATE") ret = RADIO_EQ_TYPE::GATE;
    else throw WRONG_FORMAT();
    std::stoi(cfg.ID); //check ID is number
    addRadioEq(cfg,ret);
}

void RADIO_EQ_CONTAINER::deleteRadioEq(std::string name)
{
    delete m_radioEqMap[name];
    m_radioEqMap.erase(name);
    saveConfig(my_data->server_settings->radio433MHzConfigFile);
}

RADIO_EQ* RADIO_EQ_CONTAINER::getEqPointer(std::string name)
{
    auto m = m_radioEqMap.find(name);
    if (m != m_radioEqMap.end()){
        return (m->second);
    }
    else
    {
        throw std::string("433MHz equipment not found "+name);
    }
}

std::vector<RADIO_SWITCH*> RADIO_EQ_CONTAINER::getSwitchPointerVector()
{
    std::vector<RADIO_SWITCH*> switchVector;

    for (auto it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::SWITCH){
            switchVector.push_back(static_cast<RADIO_SWITCH*>(it.second));
        }
    }
    return switchVector;
}

std::vector<RADIO_BUTTON *> RADIO_EQ_CONTAINER::getButtonPointerVector()
{
    std::vector<RADIO_BUTTON*> buttonVector;

    for (auto it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::BUTTON){
            buttonVector.push_back(static_cast<RADIO_BUTTON*>(it.second));
        }
    }
    return buttonVector;
}

std::vector<RADIO_WEATHER_STATION *> RADIO_EQ_CONTAINER::getWeather_StationPtrVector()
{
    std::vector<RADIO_WEATHER_STATION*> weatherStVe;
    for (auto it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::WEATHER_S){
            weatherStVe.push_back(static_cast<RADIO_WEATHER_STATION*>(it.second));
        }
    }
    return weatherStVe;
}

std::string RADIO_EQ_CONTAINER::listAllName()
{
    std::string allName;

    for(auto it = m_radioEqMap.begin(); it != m_radioEqMap.end(); ++it) {
        allName.append(it->first);
        allName.append("\t ID: ");
        allName.append( it->second->getID()  );
        allName.append("\t state: ");
        allName.append(stateToString(it->second->getState()  ));
        allName.append("\n");
    }

    return allName;
}

bool RADIO_EQ_CONTAINER::nameExist(std::string name)
{
    bool exist = false;
    if(m_radioEqMap.find(name) != m_radioEqMap.end())
    {
        exist = true;
    }
    return exist;
}

void RADIO_EQ_CONTAINER::loadConfig(std::string filePath)
{
    std::ifstream myfile (filePath);
    if (myfile.is_open())
    {
        nlohmann::json j;
        myfile >> j;

        RADIO_EQ_CONFIG cfg;

        try
        {
            nlohmann::json switchJson = j.at("SWITCH");
            for (nlohmann::json::iterator it = switchJson.begin(); it != switchJson.end(); ++it)
            {
                nlohmann::json switchJson =  it.value();
                cfg.name = switchJson.at("name").get<std::string>();
                cfg.ID   = switchJson.at("id").get<std::string>();
                cfg.offCode = switchJson.at("OFF").get<std::string>();
                cfg.onCode  = switchJson.at("ON").get<std::string>();
                cfg.on15sec = switchJson.at("on15sec").get<std::string>();
                cfg.sunrise = switchJson.at("sunrise").get<std::string>();
                cfg.sunset  = switchJson.at("sunset").get<std::string>();
                cfg.type    = switchJson.at("type").get<std::string>();
                addRadioEq(cfg,RADIO_EQ_TYPE::SWITCH);
                dynamic_cast<RADIO_SWITCH*>(getEqPointer(cfg.name))->setCode(cfg);
            }
        }
        catch(...)
        {
#ifndef BT_TEST
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "no SWITCH equipment in config" <<   std::endl;
        log_file_mutex.mutex_unlock();
#else
            std::cout << "no SWITCH equipment in config" <<std::endl;
#endif
        }

        try
        {
            nlohmann::json buttonJson = j.at("BUTTON");
            for (nlohmann::json::iterator it = buttonJson.begin(); it != buttonJson.end(); ++it)
            {
                nlohmann::json buttonJson =  it.value();
                cfg.name = buttonJson.at("name").get<std::string>();
                cfg.ID   = buttonJson.at("id").get<std::string>();
                cfg.offCode = buttonJson.at("OFF").get<std::string>();
                cfg.onCode  = buttonJson.at("ON").get<std::string>();
                cfg.type    = buttonJson.at("type").get<std::string>();
                addRadioEq(cfg,RADIO_EQ_TYPE::BUTTON);
            }
        }
        catch(...)
        {
#ifndef BT_TEST
            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "no BUTTONs equipment in config" <<   std::endl;
            log_file_mutex.mutex_unlock();
#else
            std::cout << "no BUTTONs equipment in config"  <<std::endl;
#endif
        }

        try
        {
            nlohmann::json weatherJson= j.at("WEATHER");
            for (nlohmann::json::iterator it = weatherJson.begin(); it != weatherJson.end(); ++it)
            {
                nlohmann::json weatherJson =  it.value();
                cfg.name = weatherJson.at("name").get<std::string>();
                cfg.ID   = weatherJson.at("id").get<std::string>();
                cfg.type    = weatherJson.at("type").get<std::string>();
                addRadioEq(cfg,RADIO_EQ_TYPE::WEATHER_S);
            }
        }
        catch(...)
        {
#ifndef BT_TEST
            log_file_mutex.mutex_lock();
            log_file_cout << DEBUG << "no WEATHER STAIONs equipment in config" <<   std::endl;
            log_file_mutex.mutex_unlock();
#else
            std::cout << "no  WEATHER STAIONs  equipment in config"  <<std::endl;
#endif
        }

        myfile.close();
    }
    else std::cout << "Unable to open file";
}

void RADIO_EQ_CONTAINER::saveConfig(std::string filePath)
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
    m_configJson["WEATHER"] = weatherJson ;
    // write prettified JSON to another file
    std::ofstream o(filePath);
    o << std::setw(4) << m_configJson << std::endl;
}

RADIO_EQ::RADIO_EQ()
{
    puts("RADIO_EQ::RADIO_EQ()");
}

RADIO_EQ::~RADIO_EQ()
{
    puts("RADIO_EQ::~RADIO_EQ()");
}

RADIO_EQ_TYPE RADIO_EQ::getType()
{
    return m_type;
}

RADIO_WEATHER_STATION::RADIO_WEATHER_STATION(thread_data *my_data, RADIO_EQ_CONFIG cfg, RADIO_EQ_TYPE type)
{
    puts("RADIO_WEATHER_STATION::RADIO_WEATHER_STATION()");
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
    RADIO_EQ::m_config  = cfg;
}

RADIO_WEATHER_STATION::~RADIO_WEATHER_STATION()
{
    puts("RADIO_WEATHER_STATION::~RADIO_WEATHER_STATION()");
}

STATE RADIO_WEATHER_STATION::getState()
{
    return m_state;
}

std::string RADIO_WEATHER_STATION::getName()
{
    return RADIO_EQ::m_config.name;
}

std::string RADIO_WEATHER_STATION::getID()
{
    return RADIO_EQ::m_config.ID;
}
