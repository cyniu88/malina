#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_shed.h"
#include "../../iDom_server_OOP/src/iDomTools/mock/iDomToolsMock.h"

class command_shed_fixture2 : public testing::Test
{
public:
    command_shed_fixture2()
        : test_command_shed(std::make_unique<command_shed>("shed")),
          main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.m_keyHandler  = std::make_unique<iDomKEY_ACCESS>("");
        test_context.mqttHandler   = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
        test_context.main_iDomTools = main_iDomTools;

        // bazowa konfiguracja potrzebna dla gałęzi "put"
        test_context.lusina.shedConfJson = nlohmann::json::parse(R"({
            "deepSleep": false,
            "howLongDeepSleep": 60,
            "fanON": false
        })");
    }

protected:
    std::unique_ptr<command_shed> test_command_shed;
    std::vector<std::string> test_v;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    thread_context test_context;

    const std::string valid_json = R"({"acdc":3.9,"temperatura":22.5,"ciśnienie":1001.0,"wilgotność":55.0,"millis":5000,"bateria":3.8,"podłoga":21.0})";
};

// ──────────────────────────────────────────────────────────────────────────────
// brak parametru (tylko "shed")
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_shed_fixture2, no_param)
{
    test_v = {"shed"};
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "command shed - wrong paramiter");
}

// ──────────────────────────────────────────────────────────────────────────────
// put – nieprawidłowy JSON
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_shed_fixture2, put_invalid_json)
{
    test_v = {"shed", "put", "to nie jest json {{{"};
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "error 222");
}

// ──────────────────────────────────────────────────────────────────────────────
// set deepSleep
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_shed_fixture2, set_deepSleep_true)
{
    test_v = {"shed", "set", "deepSleep", "true", "120"};
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("ustawiono deep sleep"));
    EXPECT_TRUE(test_context.lusina.shedConfJson["deepSleep"].get<bool>());
    EXPECT_EQ(test_context.lusina.shedConfJson["howLongDeepSleep"].get<int>(), 120);
}

TEST_F(command_shed_fixture2, set_deepSleep_false)
{
    test_v = {"shed", "set", "deepSleep", "false", "30"};
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("ustawiono deep sleep"));
    EXPECT_FALSE(test_context.lusina.shedConfJson["deepSleep"].get<bool>());
    EXPECT_EQ(test_context.lusina.shedConfJson["howLongDeepSleep"].get<int>(), 30);
}

// ──────────────────────────────────────────────────────────────────────────────
// put + show – dane trafiają do shedJson i da się je odczytać
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_shed_fixture2, put_then_show_contains_temperature)
{
    EXPECT_CALL(*main_iDomTools.get(), isItDay()).WillOnce(testing::Return(false));

    test_v = {"shed", "put", valid_json};
    auto ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "DONE");

    test_v = {"shed", "show"};
    ret = test_command_shed->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("temperatura"));
}

// ──────────────────────────────────────────────────────────────────────────────
// put – dane trafiają do wektorów shedFloor, shedTemp itp.
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_shed_fixture2, put_fills_vectors)
{
    EXPECT_CALL(*main_iDomTools.get(), isItDay()).WillOnce(testing::Return(true));

    test_v = {"shed", "put", valid_json};
    test_command_shed->execute(test_v, &test_context);

    EXPECT_FALSE(test_context.lusina.shedFloor.empty());
    EXPECT_FALSE(test_context.lusina.shedTemp.empty());
    EXPECT_FALSE(test_context.lusina.shedHum.empty());
    EXPECT_FALSE(test_context.lusina.shedPres.empty());
    EXPECT_FALSE(test_context.lusina.acdc.empty());
}

// ──────────────────────────────────────────────────────────────────────────────
// help
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_shed_fixture2, help_contains_keywords)
{
    std::string h = test_command_shed->help();
    EXPECT_THAT(h, testing::HasSubstr("put"));
    EXPECT_THAT(h, testing::HasSubstr("show"));
    EXPECT_THAT(h, testing::HasSubstr("deepSleep"));
}
