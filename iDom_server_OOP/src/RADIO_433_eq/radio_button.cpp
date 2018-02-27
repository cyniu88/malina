#include "radio_433_eq.h"

RADIO_BUTTON::RADIO_BUTTON(thread_data *my_data, std::string name, std::string id, RADIO_EQ_TYPE type):
    m_name(name),
    m_id(id)
{
    puts(" konstruktor RADIO_BUTTON");
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
}

RADIO_BUTTON::~RADIO_BUTTON()
{
    puts("\ndestruktor ~RADIO_BUTTON()\n");
}

STATE RADIO_BUTTON::getState()
{
    return m_state;
}

std::string RADIO_BUTTON::getName()
{
    return m_name;
}

std::string RADIO_BUTTON::getID()
{
    return m_id;
}
