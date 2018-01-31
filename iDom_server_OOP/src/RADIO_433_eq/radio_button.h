#ifndef RADIO_BUTTON_H
#define RADIO_BUTTON_H

#include "radio_433_eq.h"

class RADIO_BUTTON: public RADIO_EQ
{
    std::string m_name;
    std::string m_id;
    STATE m_state = STATE::UNDEFINE;
    int m_onCode =  2004;
    int m_offCode = 2008;
public:
    RADIO_BUTTON(thread_data * my_data, std::string name, std::string id, RADIO_EQ_TYPE type);
    ~RADIO_BUTTON();
    STATE getState();
    std::string getName();
    std::string getID();

};

#endif // RADIO_BUTTON_H
