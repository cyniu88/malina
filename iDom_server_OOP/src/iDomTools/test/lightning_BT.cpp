#include <gtest/gtest.h>

#include "/home/pi/programowanie/iDom_server_OOP/test/iDom_TESTs-CMAKE/test_data.h"
#include "testJSON.h"
#include "../idomtools.h"

TEST(lightning_Class, lightningAlertOFF)
{
    TEST_JSON test_Json;
    LIGHTNING test_lightning;

    CARDINAL_DIRECTIONS::ALARM_INFO test_struct =
            test_lightning.lightningAlert(test_Json.jj_noLightning);

    std::cout <<std::endl << test_struct.data.str();
    EXPECT_FALSE(test_struct.riseAlarm) << "BRAK ALARMU";
}

TEST(lightning_Class, lightningAlertON)
{
    TEST_JSON test_Json;
    LIGHTNING test_lightning;

    CARDINAL_DIRECTIONS::ALARM_INFO test_struct =
            test_lightning.lightningAlert(test_Json.jj_lightning);

    std::cout <<std::endl << test_struct.data.str();
    EXPECT_TRUE(test_struct.riseAlarm) << "BRAK ALARMU";
}

TEST(lightning_Class, checkLightningAlert)
{
    LIGHTNING test_lightning;
    TEST_JSON test_Json;
    CARDINAL_DIRECTIONS::ALARM_INFO test_struct =
            test_lightning.lightningAlert(test_Json.jj_noLightning);

    //std::cout <<std::endl << test_struct.data.str();

    bool test_result = test_lightning.checkLightningAlert(&test_struct);

    EXPECT_FALSE(test_result) << "BRAK ALARMU";
    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result) << "BRAK ALARMU";
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU";
    test_struct.riseAlarm = false;
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU";
    test_struct.riseAlarm = false;
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU";
}
