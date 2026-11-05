#include <gtest/gtest.h>
#include "../cron.hpp"
#include "../../iDom_server_OOP.h"
#include "../../functions/functions.h"
#include "../../iDomTools/mock/iDomToolsMock.h"
#include "../../buderus/mock/buderus_mock.h"
#include <thread>
#include <iostream>

class cron_Class_fixture : public ::testing::Test
{
public:
    cron_Class_fixture() : main_iDomTools(std::make_shared<iDomToolsMock>()),
                           buderusMock(std::make_shared<BUDERUS_MOCK>())
    {
        test_context.server_settings = &testCS;
        test_context.server_settings->_cron = "../config/cron.yaml";
        test_context.idom_all_state.houseState = STATE::LOCK;
        test_context.main_iDomTools = main_iDomTools;
        test_context.main_Rs232 = std::make_unique<SerialPi>("cyniu");
        test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("cyniu");
        test_q = std::make_unique<CRON>(&test_context);
        test_context.main_house_room_handler = std::make_shared<house_room_handler>(&test_context);
        test_context.ptr_buderus = buderusMock;
    }

protected:
    thread_context test_context;
    CONFIG_JSON testCS;
    std::unique_ptr<CRON> test_q;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    std::shared_ptr<BUDERUS_MOCK> buderusMock;

    void setCommonExpectations(int runCommandTimes = 0, const std::vector<std::string> &commands = {})
    {
        if (runCommandTimes > 0)
        {
            if (!commands.empty())
            {
                EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(commands))
                    .Times(runCommandTimes)
                    .WillRepeatedly([](const std::vector<std::string> &v)
                                    {
                        for (const auto &cmd : v)
                            std::cout << "Executed command: " << cmd << std::endl; });
            }
            else
            {
                EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(::testing::_))
                    .Times(runCommandTimes)
                    .WillRepeatedly([](const std::vector<std::string> &v)
                                    {
                        for (const auto &cmd : v)
                            std::cout << "Executed command: " << cmd << std::endl; });
            }
        }
        EXPECT_CALL(*main_iDomTools.get(), checkAlarm())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Alarm checked." << std::endl; });
        EXPECT_CALL(*main_iDomTools.get(), updateTemperatureStats())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Temperature stats updated." << std::endl; });
        EXPECT_CALL(*buderusMock.get(), circlePompToRun())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Circle pump activated." << std::endl; });
        EXPECT_CALL(*main_iDomTools.get(), getSunsetClock())
            .Times(1)
            .WillOnce([]()
                      { return Clock(18, 30); });
        EXPECT_CALL(*main_iDomTools.get(), getSunriseClock())
            .Times(1)
            .WillOnce([]()
                      { return Clock(6, 30); });
        EXPECT_CALL(*main_iDomTools.get(), send_data_to_thingSpeak())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Data sent to ThingSpeak." << std::endl; });
        EXPECT_CALL(*main_iDomTools.get(), healthCheck())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Health check performed." << std::endl; });
        EXPECT_CALL(*main_iDomTools.get(), send_data_to_influxdb())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Data sent to InfluxDB." << std::endl; });
        EXPECT_CALL(*main_iDomTools.get(), uploadRamCpuUsage())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "RAM and CPU usage uploaded." << std::endl; });
        EXPECT_CALL(*main_iDomTools.get(), uploadRecuperatorData())
            .Times(1)
            .WillOnce([]()
                      { std::cout << "Recuperator data uploaded." << std::endl; });
    }
};

TEST_F(cron_Class_fixture, empty_yaml)
{
    test_context.server_settings->_cron = "../config/cron_empty.yaml";
    std::cout << "Testing empty YAML file... " << test_context.server_settings->_cron << std::endl;
    auto test_q2 = std::make_unique<CRON>(&test_context);
    test_q2->runCommandCron("30min");
}

TEST_F(cron_Class_fixture, runCommandCron)
{
    Clock::setTime_forBT_usage(12, 12);
    useful_F::go_while = true;
    EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(::testing::_))
        .Times(1)
        .WillRepeatedly([](const std::vector<std::string> &v)
                        {
                        for (const auto &cmd : v)
                            std::cout << "Executed command: " << cmd << std::endl; });
    test_q->runCommandCron("30min");
}

TEST_F(cron_Class_fixture, run)
{
    Clock::setTime_forBT_usage(12, 00);
    useful_F::go_while = true;
    setCommonExpectations(5);
    test_q->run();
}

TEST_F(cron_Class_fixture, run_empty)
{
    std::cout << "Testing empty YAML file... " << test_context.server_settings->_cron << std::endl;
    test_context.server_settings->_cron = "../config/cron_empty.yaml";
    auto test_q2 = std::make_unique<CRON>(&test_context);
    Clock::setTime_forBT_usage(12, 00);
    useful_F::go_while = true;
    setCommonExpectations();
    test_q2->run();
}

TEST_F(cron_Class_fixture, run_one_yaml)
{
    std::cout << "Testing empty YAML file... " << test_context.server_settings->_cron << std::endl;
    test_context.server_settings->_cron = "../config/cron_one.yaml";
    auto test_q2 = std::make_unique<CRON>(&test_context);
    Clock::setTime_forBT_usage(12, 00);
    useful_F::go_while = true;
    setCommonExpectations(1, {"iDom mqtt cyniu/test test"});
    test_q2->run();
}

TEST_F(cron_Class_fixture, runCommandCron_unlock_state)
{
    // pokrycie gałęzi UNLOCK w runCommandCron()
    test_context.idom_all_state.houseState = STATE::UNLOCK;
    Clock::setTime_forBT_usage(12, 12);
    useful_F::go_while = true;
    EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(::testing::_))
        .Times(1)
        .WillRepeatedly([](const std::vector<std::string> &v)
                        {
                            for (const auto &cmd : v)
                                std::cout << "Executed command: " << cmd << std::endl; });
    test_q->runCommandCron("30min");
}

TEST_F(cron_Class_fixture, runCommandCron_unknown_time_key)
{
    // czas którego nie ma w yamlu – nie powinien nic wołać
    Clock::setTime_forBT_usage(12, 12);
    useful_F::go_while = true;
    // brak EXPECT_CALL – runCommandFromJson NIE powinien zostać wywołany
    test_q->runCommandCron("never");
}

TEST_F(cron_Class_fixture, run_triggers_sunrise)
{
    // run() o godzinie wschodu – pokrywa runOnSunrise() i runEveryone_1min()
    Clock::setTime_forBT_usage(6, 30);
    useful_F::go_while = true;

    EXPECT_CALL(*main_iDomTools.get(), getSunriseClock())
        .Times(1)
        .WillOnce([]() { return Clock(6, 30); });
    EXPECT_CALL(*main_iDomTools.get(), getSunsetClock())
        .Times(1)
        .WillOnce([]() { return Clock(18, 30); });
    EXPECT_CALL(*main_iDomTools.get(), runOnSunrise()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), checkAlarm()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), updateTemperatureStats()).Times(1);
    EXPECT_CALL(*buderusMock.get(), circlePompToRun()).Times(1);
    // minuta 30 → też odpala 5min, 15min, 30min
    EXPECT_CALL(*main_iDomTools.get(), healthCheck()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), send_data_to_influxdb()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), uploadRamCpuUsage()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), uploadRecuperatorData()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), send_data_to_thingSpeak()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(::testing::_)).Times(::testing::AnyNumber());

    test_q->run();
}

TEST_F(cron_Class_fixture, run_triggers_sunset)
{
    // run() o godzinie zachodu – pokrywa runOnSunset() i runEveryone_1min()
    Clock::setTime_forBT_usage(18, 30);
    useful_F::go_while = true;

    EXPECT_CALL(*main_iDomTools.get(), getSunsetClock())
        .Times(1)
        .WillOnce([]() { return Clock(18, 30); });
    EXPECT_CALL(*main_iDomTools.get(), getSunriseClock())
        .Times(1)
        .WillOnce([]() { return Clock(6, 30); });
    EXPECT_CALL(*main_iDomTools.get(), runOnSunset()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), checkAlarm()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), updateTemperatureStats()).Times(1);
    EXPECT_CALL(*buderusMock.get(), circlePompToRun()).Times(1);
    // minuta 30 → też odpala 5min, 15min, 30min
    EXPECT_CALL(*main_iDomTools.get(), healthCheck()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), send_data_to_influxdb()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), uploadRamCpuUsage()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), uploadRecuperatorData()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), send_data_to_thingSpeak()).Times(1);
    EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(::testing::_)).Times(::testing::AnyNumber());

    test_q->run();
}

TEST_F(cron_Class_fixture, run_at_top_of_hour_covers_1h)
{
    // minuta 0 → odpala 1min + 5min + 15min + 30min + 1h
    Clock::setTime_forBT_usage(8, 0);
    useful_F::go_while = true;
    setCommonExpectations(5);
    test_q->run();
}
