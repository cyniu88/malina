#ifndef BUDERUS_H
#define BUDERUS_H

#include <mutex>

#include "../json2/json.hpp"
#include "../idom_api.h"

class BUDERUS: public iDom_API
{
    std::mutex m_lockGuard;
    nlohmann::json m_boiler_data;
    nlohmann::json m_thermostat_data;
    bool m_tapwater_active = false;
    bool m_heating_active = false;

public:
    BUDERUS();
    ~BUDERUS();
    void updateBoilerDataFromMQTT(nlohmann::json jj);
    void updateThermostatDataFromMQTT(nlohmann::json jj);
    void tapWaterActivated();
    void tapWaterDeactivated();
    void heatingActivated();
    void heatingDeactivated();
    void setHeating(bool b);
    void setTapWater(bool b);

    bool isHeatingActiv();
    bool isTapWaterActiv();

    std::string dump() const override;
};

#endif // BUDERUS_H
