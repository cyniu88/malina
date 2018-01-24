#include <iostream>
#include <fstream>
#include <sstream>
#include "radio_433_eq.h"

RADIO_SWITCH::RADIO_SWITCH(thread_data *my_data, std::string name, std::string id, RADIO_EQ_TYPE type):
    main433MHz(my_data),
    m_name(name),
    m_id(id)
{
    puts(" konstruktor RADIO_SWITCH");
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
}

RADIO_SWITCH::~RADIO_SWITCH()
{
    puts("\ndestruktor ~RADIO_SWITCH()\n");
}

void RADIO_SWITCH::on()
{
    main433MHz.sendCode(m_id + std::to_string(m_onCode));
    m_state = STATE::ON;
    RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(m_name,STATE::ON);
}

void RADIO_SWITCH::off()
{
    main433MHz.sendCode(m_id + std::to_string(m_offCode));
    m_state = STATE::OFF;
    RADIO_EQ::m_my_data->main_iDomStatus->setObjectState(m_name,STATE::OFF);
}

void RADIO_SWITCH::onFor15sec()
{
    main433MHz.sendCode(m_id + std::to_string(m_onFor15secCode));
    m_state = STATE::WORKING;
}

STATE RADIO_SWITCH::getState()
{
    return m_state;
}

std::string RADIO_SWITCH::getName()
{
    return m_name;
}

std::string RADIO_SWITCH::getID()
{
    return m_id;
}

void RADIO_SWITCH::setCode(RADIO_EQ_CONFIG cfg)
{
    if(cfg.onCode > 0){
        m_onCode = cfg.onCode;
    }
    if(cfg.offCode > 0){
        m_offCode = cfg.offCode;
    }
    if(cfg.on15sec > 0){
        m_onFor15secCode = cfg.on15sec;
    }
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
    for(auto it = m_radioEqMap.begin(); it != m_radioEqMap.end(); ++it) {
        delete it->second;
    }
}

void RADIO_EQ_CONTAINER::addRadioEq(std::string name,std::string id, RADIO_EQ_TYPE type)
{
    switch (type) {
    case RADIO_EQ_TYPE::SWITCH:
        m_radioEqMap.insert(std::make_pair(name, new RADIO_SWITCH(this->my_data, name, id, type)  )    );
        break;
    default:
        break;
    }
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

std::vector<RADIO_SWITCH *> RADIO_EQ_CONTAINER::getSwitchPointerVector()
{
    std::vector<RADIO_SWITCH*> switchVector;

    for (auto it : m_radioEqMap){
        if (it.second->getType() == RADIO_EQ_TYPE::SWITCH){
            puts("\n to jest switch\n");
            switchVector.push_back(static_cast<RADIO_SWITCH*>(it.second));
        }
    }
    return switchVector;
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

void RADIO_EQ_CONTAINER::loadConfig(std::string filePath)
{

    std::stringstream lineStream;
    std::string line;
    RADIO_EQ_CONFIG cfg;

    std::ifstream myfile (filePath);
    if (myfile.is_open())
    {
        while ( getline (myfile, line) )
        {
            std::cout << line << std::endl;
            if (line.front() != '#'){
                lineStream << line;
                std::string sunrise, sunset;
                lineStream >> cfg.type >> cfg.name >> cfg.ID >> cfg.onCode >>cfg.offCode >> cfg.on15sec >> cfg.sunrise >> cfg.sunset ;
                if (cfg.type == "switch"){
                    addRadioEq(cfg.name,cfg.ID,RADIO_EQ_TYPE::SWITCH);
                    dynamic_cast<RADIO_SWITCH*>(getEqPointer(cfg.name))->setCode(cfg);
                }
                //NOTE add more type
            }
            lineStream.clear();
        }
        myfile.close();
    }

    else std::cout << "Unable to open file";
}

RADIO_EQ::RADIO_EQ()
{
    puts("konstruktor RADIO_EQ()");
}

RADIO_EQ::~RADIO_EQ()
{
    puts("destruktor ~RADIO_EQ()");
}

RADIO_EQ_TYPE RADIO_EQ::getType()
{
    return m_type;
}
