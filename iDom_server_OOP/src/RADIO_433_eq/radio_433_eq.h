#pragma once

#include <map>
#include <optional>

#include "../433MHz/RFLink/rflinkhandler.h"
#include "../433MHz/rc_433mhz.h"
#include "json.hpp"

struct WEATHER_STRUCT : public iDom_API
{
private:
    unsigned long int m_counter = 0;

protected:
    std::optional<unsigned int> m_humidity;
    std::optional<double> m_temperature;
    std::optional<unsigned int> m_barometricPressure;

public:
    WEATHER_STRUCT()
    {
        m_className.append(typeid(this).name());
        addToMap(m_className, this);
    }
    ~WEATHER_STRUCT()
    {
        removeFromMap(m_className);
    }

    std::optional<unsigned int> getHumidity() { return m_humidity; }
    std::optional<double> getTemperature() { return m_temperature; }
    std::optional<unsigned int> getBarometricPressure() { return m_barometricPressure; }
    std::string getDataString()
    {
        return "data: " + std::to_string(m_counter) + "\n" + "Humidity=" + std::to_string(getHumidity().value_or(-1)) + "%\n" +
               "temperature= " + to_string_with_precision(getTemperature().value_or(0)) + "c\n" +
               "Pressure= " + std::to_string(getBarometricPressure().value_or(0)) + "kPa\n";
    }

    void putData(std::string data)
    {
        std::string tempStr;
        ++m_counter;
        try
        {
            m_humidity = std::stoi(RFLinkHandler::getArgumentValueFromRFLinkMSG(data, "HUM"));
        }
        catch (...)
        {
        }
        try
        {
            m_barometricPressure = std::stoi(RFLinkHandler::getArgumentValueFromRFLinkMSG(data, "BARO"));
        }
        catch (...)
        {
        }
        try
        {
            int t = 0;
            tempStr = RFLinkHandler::getArgumentValueFromRFLinkMSG(data, "TEMP");
            std::stringstream ss;
            ss << std::hex << tempStr.substr(tempStr.size() - 3, tempStr.size());
            ss >> t;
            m_temperature = t / 10.0;
            if (tempStr.at(0) == '8')
            {
                m_temperature = m_temperature.value() * -1.0;
            }
        }
        catch (...)
        {
        }
        // std::cout << "DUPA: "<<data<<" temp=" << m_temperature<< " hum="<<m_humidity<< std::endl;
    }
    std::string dump() const override;
};

enum class RADIO_EQ_TYPE
{
    SWITCH = 1,
    PIR,
    GATE,
    BUTTON,
    WEATHER_S,
    NONE
};
struct RADIO_EQ_CONFIG
{
    std::string name = "NULL";
    std::string ID = "NULL";
    std::string type = "NULL";
    std::string onCode = "NULL";
    std::string offCode = "NULL";
    std::string on15sec = "NULL";
    std::string commandRun = "NULL";
    void set(const std::string &type,
             const std::string &name,
             const std::string &ID,
             const std::string &onCode = "null",
             const std::string &offCode = "null",
             const std::string &on15sec = "null",
             const std::string &commandRun = "null")
    {
        this->name = name;
        this->ID = ID;
        this->type = type;
        this->onCode = onCode;
        this->offCode = offCode;
        this->on15sec = on15sec;
        this->commandRun = commandRun;
        // add to getJson below
    }

    nlohmann::json getJson()
    {
        nlohmann::json jj;
        jj["name"] = name;
        jj["id"] = ID;
        jj["type"] = type;
        jj["ON"] = onCode;
        jj["OFF"] = offCode;
        jj["on15sec"] = on15sec;
        jj["commandRun"] = commandRun;
        return jj;
    }
};

class RADIO_EQ
{
public:
    RADIO_EQ();
    virtual ~RADIO_EQ() = default;
    virtual STATE getState() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getID() const = 0;
    virtual RADIO_EQ_TYPE getType() const;

protected:
    thread_data *m_context;
    RADIO_EQ_TYPE m_type;

public:
    RADIO_EQ_CONFIG m_config;
};
class RADIO_WEATHER_STATION : public RADIO_EQ
{
    STATE m_state = STATE::UNDEFINE;

public:
    RADIO_WEATHER_STATION(thread_data *context, const RADIO_EQ_CONFIG &cfg, RADIO_EQ_TYPE type);
    ~RADIO_WEATHER_STATION() = default;
    STATE getState() const override;
    std::string getName() const override;
    std::string getID() const override;
    // data
    WEATHER_STRUCT data;

private:
    RADIO_WEATHER_STATION();
};
class RADIO_BUTTON : public RADIO_EQ
{
    STATE m_state = STATE::UNDEFINE;

public:
    RADIO_BUTTON(thread_data *context, const RADIO_EQ_CONFIG &cfg, RADIO_EQ_TYPE type);
    ~RADIO_BUTTON() = default;
    STATE getState() const override;
    void setState(STATE s);
    std::string getName() const override;
    std::string getID() const override;
    std::string getCommandRun() const;

private:
    RADIO_BUTTON();
};

class RADIO_SWITCH : public RADIO_EQ
{
#ifdef BT_TEST
public:
#endif
    RC_433MHz main433MHz;
    STATE m_state = STATE::UNDEFINE;

public:
    RADIO_SWITCH(thread_data *context, const RADIO_EQ_CONFIG &cfg, RADIO_EQ_TYPE type);
    ~RADIO_SWITCH() = default;
    void on();
    void off();
    void on_Off();
    void onFor15sec();
    STATE getState() const override;
    std::string getName() const override;
    std::string getID() const override;
    void setCode(RADIO_EQ_CONFIG cfg);
    STATE m_sunrise = STATE::UNDEFINE;
    STATE m_sunset = STATE::UNDEFINE;

private:
    RADIO_SWITCH();
};

class RADIO_EQ_CONTAINER
{
    std::map<std::string, RADIO_EQ *> m_radioEqMap;
    thread_data *context;
    nlohmann::json m_configJson;
    RADIO_EQ_CONTAINER();

public:
    explicit RADIO_EQ_CONTAINER(thread_data *context);
    virtual ~RADIO_EQ_CONTAINER();
    void addRadioEq(RADIO_EQ_CONFIG cfg, RADIO_EQ_TYPE type);
    void addRadioEq(RADIO_EQ_CONFIG cfg, const std::string &type);
    void deleteRadioEq(const std::string &name);
    virtual RADIO_EQ *getEqPointer(std::string name);
    std::vector<RADIO_SWITCH *> getSwitchPointerVector();
    std::vector<RADIO_BUTTON *> getButtonPointerVector();
    std::vector<RADIO_WEATHER_STATION *> getWeather_StationPtrVector();
    std::string listAllName();
    bool nameExist(const std::string &name);
    void loadConfig(const std::string &filePath);
    void saveConfig(const std::string &filePath);
    std::string showConfig(const std::string &filePath);
};
