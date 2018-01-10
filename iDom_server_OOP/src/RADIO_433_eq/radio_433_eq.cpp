#include "radio_433_eq.h"

RADIO_SWITCH::RADIO_SWITCH(thread_data *my_data, std::string name):
    main433MHz(my_data),
    m_name(name)
{
    puts("wyszedl w konstruktor RADIO_SWITCH");
    std::cout <<  my_data<< std::endl;

}

RADIO_SWITCH::~RADIO_SWITCH()
{
    puts("\ndestruktor ~RADIO_SWITCH()\n");
}

void RADIO_SWITCH::on()
{
    main433MHz.sendCode(m_nameHEX + std::to_string(m_onCode));
    m_state = STATE::ON;
    puts("wychodze z RADIO_SWITCH::on() ");
}

void RADIO_SWITCH::off()
{
    main433MHz.sendCode(m_nameHEX + std::to_string(m_offCode));
    m_state = STATE::OFF;
}

void RADIO_SWITCH::onFor15sec()
{
    main433MHz.sendCode(m_nameHEX + std::to_string(m_onFor15secCode));
    m_state = STATE::WORKING;
}

RADIO_EQ_CONTAINER::RADIO_EQ_CONTAINER(thread_data *my_data)
{

   this->my_data = my_data;

}

void RADIO_EQ_CONTAINER::addRadioEq(std::string name, RADIO_EQ_TYPE type)
{
    switch (type) {
    case RADIO_EQ_TYPE::SWITCH:
        m_radioEqMap.insert(std::make_pair(name,  RADIO_SWITCH(this->my_data, name)  )    );
        break;
    default:
        break;
    }
puts("koniec metody addRadioEq");
}

RADIO_EQ *RADIO_EQ_CONTAINER::getEqPointer(std::string name)
{
    auto m = m_radioEqMap.find(name);
    if (m != m_radioEqMap.end()){
        puts("wyszedl w getEqPointer");
        std::cout <<  this->my_data<< std::endl;
       return &(m->second);
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
      allName.append("\n");
    }

    return allName;
}

RADIO_EQ::~RADIO_EQ()
{

}
