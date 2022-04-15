#ifndef BUDERUS_MOCK_H
#define BUDERUS_MOCK_H
#include<gmock/gmock.h>

#include "../buderus.h"

class BUDERUS_MOCK : public BUDERUS_IF
{
public:
    ~BUDERUS_MOCK() = default;
    MOCK_METHOD1(updateBoilerDataFromMQTT, void(nlohmann::json jj));
    MOCK_METHOD1(updateThermostatDataFromMQTT, void(nlohmann::json jj));
    MOCK_METHOD0(tapWaterActivated, void());
    MOCK_METHOD0(tapWaterDeactivated, void());
    MOCK_METHOD0(heatingActivated, void());
    MOCK_METHOD0(heatingDeactivated, void());
    MOCK_METHOD1(setHeating, void(bool b));
    MOCK_METHOD1(setTapWater, void(bool b));

    MOCK_METHOD0(isHeatingActiv, bool());
    MOCK_METHOD0(isTapWaterActiv, bool());

    MOCK_CONST_METHOD0(getAllData, std::string());

    MOCK_CONST_METHOD0(getOutdoorTemp, double());
    MOCK_CONST_METHOD0(getInsideTemp, double());
    MOCK_CONST_METHOD0(getBoilerTemp, double());
    MOCK_CONST_METHOD0(getCurFlowTemp, double());

    MOCK_METHOD0(circlePompToRun, void());
    MOCK_METHOD0(runCirclePompForWhile, void());
    MOCK_METHOD0(getCirclePumpState, STATE());

    MOCK_METHOD0(boilerHeatOneTime, void());
    MOCK_METHOD1(setTempInsideBuilding, void(const std::string &t));

    MOCK_CONST_METHOD0(getHeatingStartTime, unsigned int());
    MOCK_METHOD0(getDump,std::string());
};

#endif // BUDERUS_MOCK_H
