#pragma once

#include "../json2/json.hpp"
#include "../libs/useful/useful.h"

class BUDERUS_IF
{
public:
    virtual ~BUDERUS_IF() = default;
    virtual void updateBoilerDataFromMQTT(nlohmann::json jj) = 0;
    virtual void updateThermostatDataFromMQTT(nlohmann::json jj) = 0;
    virtual void tapWaterActivated() = 0;
    virtual void tapWaterDeactivated() = 0;
    virtual void heatingActivated() = 0;
    virtual void heatingDeactivated() = 0;
    virtual void setHeating(bool b) = 0;
    virtual void setTapWater(bool b) = 0;

    virtual bool isHeatingActiv() = 0;
    virtual bool isTapWaterActiv() = 0;

    virtual std::string getAllData() const = 0;

    virtual double getOutdoorTemp() const = 0;
    virtual double getInsideTemp() const = 0;
    virtual double getBoilerTemp() const = 0;
    virtual double getCurFlowTemp() const = 0;

    virtual void circlePompToRun() = 0;
    virtual void runCirclePompForWhile() = 0;
    virtual STATE getCirclePumpState() const = 0;

    virtual void boilerHeatOneTime() = 0;
    virtual void setTempInsideBuilding(const std::string &t) = 0;

    virtual unsigned int getHeatingStartTime() const = 0;
    virtual std::string getDump() = 0;
};

