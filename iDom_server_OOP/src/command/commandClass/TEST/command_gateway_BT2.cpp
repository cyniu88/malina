#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_gateway.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"

class command_gateway_fixture2 : public testing::Test
{
public:
    command_gateway_fixture2()
        : test_command_gateway(std::make_unique<command_gateway>("gateway")),
          main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.main_iDomTools = main_iDomTools;
        test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("test");
    }

protected:
    std::unique_ptr<command_gateway> test_command_gateway;
    std::vector<std::string> test_v;
    thread_context test_context;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
};

// ──────────────────────────────────────────────────────────────────────────────
// iDomUnlock
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_gateway_fixture2, iDomUnlock)
{
    EXPECT_CALL(*main_iDomTools.get(), unlockHome());
    test_v = {"gateway", "iDomUnlock"};
    auto ret = test_command_gateway->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("alarm wyłączony"));
}

// ──────────────────────────────────────────────────────────────────────────────
// sleep – uruchamia wątek asynchronicznie, sprawdzamy tylko zwracaną wartość
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_gateway_fixture2, sleep_returns_done)
{
    // Wywołanie: gateway sleep 0 mpd stop
    // sleep(0) = natychmiastowe uruchomienie lambdy – test nie czeka na nią
    EXPECT_CALL(*main_iDomTools.get(), runCommandFromJson(testing::_)).Times(testing::AtLeast(0));
    test_v = {"gateway", "sleep", "0", "mpd", "stop"};
    auto ret = test_command_gateway->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
    // krótkie odczekanie żeby lambda zdążyła się wykonać przed zniszczeniem mocka
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// ──────────────────────────────────────────────────────────────────────────────
// help
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_gateway_fixture2, help_contains_fan)
{
    std::string h = test_command_gateway->help();
    EXPECT_THAT(h, testing::HasSubstr("fan"));
    EXPECT_THAT(h, testing::HasSubstr("sleep"));
}

// ──────────────────────────────────────────────────────────────────────────────
// unknown parameter zwraca komunikat z help()
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_gateway_fixture2, unknown_contains_help)
{
    test_v = {"gateway", "unknown_xyz"};
    auto ret = test_command_gateway->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("unknown paramiter"));
    EXPECT_THAT(ret, testing::HasSubstr("fan"));
}
