#include <iostream>
#include <fstream>
#include <sstream>
#include "radio_433_eq.h"

RADIO_SWITCH::RADIO_SWITCH(thread_data *my_data, std::string name, std::string id):
    main433MHz(my_data),
    m_name(name),
    m_id(id)
{
    puts(" konstruktor RADIO_SWITCH");
    RADIO_EQ::m_my_data = my_data;
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

void RADIO_SWITCH::setCode(int on, int off, int for15sec)
{
    m_onCode = on;
    m_offCode = off;
    m_onFor15secCode = for15sec;
    //m_id.clear();
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
        m_radioEqMap.insert(std::make_pair(name, new RADIO_SWITCH(this->my_data, name, id)  )    );
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

                lineStream >> cfg.type >> cfg.name >> cfg.ID >> cfg.onCode >>cfg.offCode >> cfg.on15sec ;
                if (cfg.type == "switch"){
                    addRadioEq(cfg.name,cfg.ID,RADIO_EQ_TYPE::SWITCH);
                    dynamic_cast<RADIO_SWITCH*>(getEqPointer(cfg.name))->setCode(cfg.onCode,cfg.offCode,cfg.on15sec);
                }
                //TODO add more type
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
