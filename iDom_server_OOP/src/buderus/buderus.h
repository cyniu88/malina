#ifndef BUDERUS_H
#define BUDERUS_H

#include <mutex>

#include "buderus_interface.h"
#include "../json2/json.hpp"
#include "../idom_api.h"
#include "../iDom_server_OOP.h"

class BUDERUS : public BUDERUS_IF, iDom_API
{
    mutable std::mutex m_lockGuard;
    nlohmann::json m_boiler_data;
    nlohmann::json m_boiler_data_ww;
    nlohmann::json m_thermostat_data;
    bool m_tapwater_active = false;
    bool m_heating_active = false;
    unsigned int m_heating_time = 0;
    double m_outdoorTemp = 0.0;
    double m_insideTemp = 0.0;
    double m_boilerTemp = 0.0;
    double m_curFlowTemp = 0.0;
    bool m_circlePompCanRun = false;
    STATE m_circlePump = STATE::UNDEFINE;

public:
    BUDERUS();
    ~BUDERUS();
    void updateBoilerDataFromMQTT(nlohmann::json jj) override;
    void updateThermostatDataFromMQTT(nlohmann::json jj) override;
    void tapWaterActivated() override;
    void tapWaterDeactivated() override;
    void heatingActivated() override;
    void heatingDeactivated() override;
    void setHeating(bool b) override;
    void setTapWater(bool b) override;

    bool isHeatingActiv() override;
    bool isTapWaterActiv() override;

    std::string getAllData() const override;

    double getOutdoorTemp() const override;
    double getInsideTemp() const override;
    double getBoilerTemp() const override;
    double getCurFlowTemp() const override;

    void circlePompToRun() override;
    void runCirclePompForWhile() override;
    STATE getCirclePumpState() override;

    void boilerHeatOneTime() override;
    void setTempInsideBuilding(const std::string &t) override;

    unsigned int getHeatingStartTime() const override;

    std::string getDump() override;
    std::string dump() const override;
};

#endif // BUDERUS_H
