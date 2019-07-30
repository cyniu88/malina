#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test_data.h"
#include "testJSON.h"
#include "../idomtools.h"
#include "../../functions/functions.h"

class lightning_Class_fixture : public ::testing::Test
{
protected:
    TEST_JSON test_Json;
    LIGHTNING test_lightning;
    CARDINAL_DIRECTIONS::ALARM_INFO test_struct;
    thread_data test_myData;
    virtual void SetUp() final
    {
        std::cout << "konfiguracja przed testem lightning_Class " <<std::endl;
        useful_F::setStaticData(&test_myData);
    }

    virtual void TearDown() final
    {
        std::cout << "czyszczenie po tescie lightning_Class " <<std::endl;
    }
};

TEST_F(lightning_Class_fixture, lightningAlertOFF)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_noLightning);
    std::cout <<std::endl << test_struct.data.str();
    EXPECT_FALSE(test_struct.riseAlarm) << "BRAK ALARMU";
}

TEST_F(lightning_Class_fixture, lightningAlertON)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning);
    std::cout <<std::endl << test_struct.data.str();
    EXPECT_TRUE(test_struct.riseAlarm) << "BRAK ALARMU";
}

TEST_F(lightning_Class_fixture, checkLightningAlert)
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

TEST_F(lightning_Class_fixture, checkLightningAlert_stormNotEnoughCloser)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_noLightning);
    bool test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 1";

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning_lt15km);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_TRUE(test_result) << "BRAK ALARMU 2";

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning_lt15km);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 3";

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning_lt14_5km);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result) << "BRAK ALARMU 4";
}

TEST_F(lightning_Class_fixture, checkLightningAlert_stormCloser)
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

TEST_F(lightning_Class_fixture, oneLightning)
{
    test_struct = test_lightning.lightningAlert(test_Json.jj_oneLightning);
    bool test_result = test_lightning.checkLightningAlert(&test_struct);
    EXPECT_FALSE(test_result);
}

TEST_F(lightning_Class_fixture, errorCode)
{
    nlohmann::json test_Json2 = useful_F_libs::getJson("http://cyniu88.no-ip.pl/test/json/lightningUnauthorizedClient.json");

    test_struct = test_lightning.lightningAlert(test_Json2);
    bool test_result = test_lightning.checkLightningAlert(&test_struct);
    auto ret = useful_F::myStaticData->iDomAlarm.showAlarm();
    std::cout << "wynik: " << test_result << "alarmy: " << ret <<   test_Json2.dump(4) << std::endl;
    EXPECT_THAT(ret, testing::HasSubstr("unauthorized_client"));

    test_struct = test_lightning.lightningAlert(test_Json.jj_lightning);
    test_result = test_lightning.checkLightningAlert(&test_struct);
    ret = useful_F::myStaticData->iDomAlarm.showAlarm();
    EXPECT_THAT(ret, testing::HasSubstr("no alarms!"));
}
