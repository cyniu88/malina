#include <gtest/gtest.h>

#include "/home/pi/programowanie/iDom_server_OOP/test/iDom_TESTs-CMAKE/test_data.h"
#include "testJSON.h"
#include "../idomtools.h"

class lightning_Class : public ::testing::Test
{
protected:
    TEST_JSON test_Json;
    LIGHTNING test_lightning;
    CARDINAL_DIRECTIONS::ALARM_INFO test_struct;
    virtual void SetUp()
    {
        std::cout << "konfiguracja przed testem " <<std::endl;
    }

    virtual void TearDown()
    {
        std::cout << "czyszczenie po tescie " <<std::endl;
    }
};

TEST_F(lightning_Class, lightningAlertOFF)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_noLightning);
    std::cout <<std::endl << test_struct.data.str();
    EXPECT_FALSE(test_struct.riseAlarm) << "BRAK ALARMU";
}

TEST_F(lightning_Class, lightningAlertON)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning);
    std::cout <<std::endl << test_struct.data.str();
    EXPECT_TRUE(test_struct.riseAlarm) << "BRAK ALARMU";
}

TEST_F(lightning_Class, checkLightningAlert)
{

    nlohmann::json test_Json2 = useful_F_libs::getJson("http://cyniu88.no-ip.pl/test/json/on_lightning.json");
    test_struct = test_lightning.lightningAlert(test_Json.jj_noLightning);

    bool test_result = test_lightning.checkLightningAlert(&test_struct);

    EXPECT_FALSE(test_result) << "BRAK ALARMU 1";
    test_struct = test_lightning.lightningAlert(test_Json2);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result) << "BRAK ALARMU 2";
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 3";
    test_struct.riseAlarm = false;
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 4";
    test_struct.riseAlarm = false;
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 5";
}

TEST_F(lightning_Class, checkLightningAlert_stormCloser)
{
    nlohmann::json test_Json2 = useful_F_libs::getJson("http://cyniu88.no-ip.pl/test/json/on_lightning.json");

    test_struct = test_lightning.lightningAlert(test_Json.jj_noLightning);
    bool test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 1";

    test_struct = test_lightning.lightningAlert(test_Json2);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result) << "BRAK ALARMU 2";

    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 3";

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning_lt15km);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result) << "BRAK ALARMU 4";

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning_lt15km);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 5";

    test_struct = test_lightning.lightningAlert(test_Json.jj_noLightning);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 6";

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning_lt15km);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result) << "BRAK ALARMU 7";
}

TEST_F(lightning_Class, oneLightning)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_oneLightning);
    bool test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result);
}
