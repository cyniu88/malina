#ifndef RADIO_SWITCH_H
#define RADIO_SWITCH_H
#include <map>

#include "../433MHz/rc_433mhz.h"
enum class RADIO_EQ_TYPE{
    SWITCH = 1,
    PIR,
    GATE,
};
struct RADIO_EQ_CONFIG{
    std::string name;
    std::string ID;
    std::string type;
    int onCode;
    int offCode;
    int on15sec;
};

class RADIO_EQ{
public:
    RADIO_EQ();
    virtual ~RADIO_EQ();
    virtual STATE getState() = 0;
    virtual std::string getName() = 0;
    virtual std::string getID() = 0;
};

class RADIO_SWITCH: public RADIO_EQ
{
    RC_433MHz main433MHz;
    int m_onCode =  2004;
    int m_offCode = 2008;
    int m_onFor15secCode = 2020;
    std::string m_name;
    std::string m_id;
    STATE m_state = STATE::UNDEFINE;
public:
    RADIO_SWITCH(thread_data * my_data, std::string m_name, std::string id);
    ~RADIO_SWITCH();
    void on();
    void off();
    void onFor15sec();
    STATE getState();
    std::string getName();
    std::string getID();
    void setCode(int on, int off, int for15sec);
};

class RADIO_EQ_CONTAINER
{
    std::map <std::string, RADIO_EQ* > m_radioEqMap;
    thread_data * my_data;
public:
    RADIO_EQ_CONTAINER(thread_data * my_data);
    ~RADIO_EQ_CONTAINER();
    void addRadioEq(std::string name, std::string id, RADIO_EQ_TYPE type);
    RADIO_EQ* getEqPointer(std::string name);
    std::string listAllName();
    void loadConfig(std::string filePath);
};


#endif // RADIO_SWITCH_H
