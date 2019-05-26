#ifndef RADIO_SWITCH_H
#define RADIO_SWITCH_H
#include <map>
#include "../iDomStatus/idomstatus.h"
#include "../433MHz/RFLink/rflinkhandler.h"

#include "../433MHz/rc_433mhz.h"
#include "json.hpp"

struct WEATHER_STRUCT : public iDom_API
{
private:
    unsigned long int m_counter = 0;
protected:
    unsigned int m_humidity = 0;
    double m_temperature = 0.0;
    unsigned int m_barometricPressure = 0;
public:
    WEATHER_STRUCT(){
        m_className.append(typeid(this).name());
        addToMap(m_className,this);
    }
    ~WEATHER_STRUCT(){
        removeFromMap(m_className);
    }

    unsigned int getHumidity(){ return m_humidity; }
    double getTemperature(){ return m_temperature; }
    unsigned int getBarometricPressure(){ return m_barometricPressure; }
    std::string getDataString(){
        return "data: "+std::to_string(m_counter)+"\n"+"Humidity=" + std::to_string(getHumidity()) +"%\n"+
                "temperature= " + to_string_with_precision(getTemperature()) + "c\n"+
                "Pressure= " + std::to_string(getBarometricPressure())+ "kPa\n";
    }

    void putData(std::string data){
        std::string tempStr;
        int t = 0;
        ++m_counter;
        try{
            m_humidity = std::stoi( RFLinkHandler::getArgumentValueFromRFLinkMSG(data, "HUM") );
        }
        catch (...){ }
        try{
            m_barometricPressure = std::stoi( RFLinkHandler::getArgumentValueFromRFLinkMSG(data, "BARO") );
        }
        catch (...){ }
        try{
            tempStr = RFLinkHandler::getArgumentValueFromRFLinkMSG(data, "TEMP");
            std::stringstream ss;
            ss << std::hex << tempStr.substr(tempStr.size()-3,tempStr.size());
            ss >> t;
            m_temperature = t / 10.0;
            if(tempStr.at(0) == '8'){
                m_temperature *= -1.0;
            }
        }
        catch (...){ }
        //std::cout << "DUPA: "<<data<<" temp=" << m_temperature<< " hum="<<m_humidity<< std::endl;
    }
    std::string dump() const;
};

enum class RADIO_EQ_TYPE{
    SWITCH = 1,
    PIR,
    GATE,
    BUTTON,
    WEATHER_S,
    NONE
};
struct RADIO_EQ_CONFIG{
    std::string name = "NULL";
    std::string ID   = "NULL";
    std::string type = "NULL";
    std::string onCode  = "NULL";
    std::string offCode = "NULL";
    std::string on15sec = "NULL";
    std::string sunrise = "NULL";
    std::string sunset  = "NULL";
    std::string lock   = "NULL";
    std::string unlock = "NULL";
    void set(std::string type,
             std::string name,
             std::string ID,
             std::string onCode = "null",
             std::string offCode = "null",
             std::string on15sec = "null",
             std::string sunrise = "null",
             std::string sunset = "null",
             std::string lock = "null",
             std::string unlock = "null"){
        this->name = name;
        this->ID   = ID;
        this->type = type;
        this->onCode  = onCode;
        this->offCode = offCode;
        this->on15sec = on15sec;
        this->sunrise = sunrise;
        this->sunset  = sunset;
        this->lock   = lock;
        this->unlock = unlock;
    }

    nlohmann::json getJson(){
        nlohmann::json jj;
        jj["name"]  = name;
        jj["id"]    = ID;
        jj["type"]  = type;
        jj["ON"]    = onCode;
        jj["OFF"]   = offCode;
        jj["on15sec"] = on15sec;
        jj["sunrise"] = sunrise;
        jj["sunset"]  = sunset;
        jj["lock"]   = lock;
        jj["unlock"] = unlock;
        return jj;
    }
};

class RADIO_EQ{
public:
    RADIO_EQ();
    virtual ~RADIO_EQ();
    virtual STATE getState() = 0;
    virtual std::string getName() = 0;
    virtual std::string getID() = 0;
    virtual RADIO_EQ_TYPE getType();
protected:
    thread_data *m_my_data;
    RADIO_EQ_TYPE m_type;
public:
    RADIO_EQ_CONFIG m_config;
};
class RADIO_WEATHER_STATION: public RADIO_EQ
{
    STATE m_state = STATE::UNDEFINE;

public:
    RADIO_WEATHER_STATION(thread_data * my_data, const RADIO_EQ_CONFIG& cfg, RADIO_EQ_TYPE type);
    ~RADIO_WEATHER_STATION();
    STATE getState();
    std::string getName();
    std::string getID();
    // data
    WEATHER_STRUCT data;
private:
    RADIO_WEATHER_STATION();
};
class RADIO_BUTTON: public RADIO_EQ
{
    STATE m_state = STATE::UNDEFINE;

public:
    RADIO_BUTTON(thread_data * my_data, const RADIO_EQ_CONFIG& cfg, RADIO_EQ_TYPE type);
    ~RADIO_BUTTON();
    STATE getState();
    void setState(STATE s);
    std::string getName();
    std::string getID();
private:
    RADIO_BUTTON();
};

class RADIO_SWITCH: public RADIO_EQ
{
#ifdef BT_TEST
public:
#endif
    RC_433MHz main433MHz;
    STATE m_state = STATE::UNDEFINE;
public:
    RADIO_SWITCH(thread_data * my_data, const RADIO_EQ_CONFIG& cfg, RADIO_EQ_TYPE type);
    ~RADIO_SWITCH();
    void on();
    void off();
    void onFor15sec();
    void onSunrise();
    void onSunset();
    void onLockHome();
    void onUnlockHome();
    STATE getState();
    std::string getName();
    std::string getID();
    void setCode(RADIO_EQ_CONFIG cfg);
    STATE m_sunrise = STATE::UNDEFINE;
    STATE m_sunset  = STATE::UNDEFINE;
private:
    RADIO_SWITCH();
};

class RADIO_EQ_CONTAINER
{
    std::map <std::string, RADIO_EQ* > m_radioEqMap;
    thread_data * my_data;
    nlohmann::json m_configJson;
    RADIO_EQ_CONTAINER();
public:
    RADIO_EQ_CONTAINER(thread_data * my_data);
    virtual ~RADIO_EQ_CONTAINER();
    void addRadioEq(RADIO_EQ_CONFIG cfg, RADIO_EQ_TYPE type);
    void addRadioEq(RADIO_EQ_CONFIG cfg, const std::string& type);
    void deleteRadioEq(const std::string &name);
    virtual RADIO_EQ* getEqPointer(std::string name);
    std::vector<RADIO_SWITCH*> getSwitchPointerVector();
    std::vector<RADIO_BUTTON*> getButtonPointerVector();
    std::vector<RADIO_WEATHER_STATION *> getWeather_StationPtrVector();
    std::string listAllName();
    bool nameExist(const std::string &name);
    void loadConfig(const std::string &filePath);
    void saveConfig(const std::string &filePath);
    std::string showConfig(const std::string &filePath);
};

#endif // RADIO_SWITCH_H
