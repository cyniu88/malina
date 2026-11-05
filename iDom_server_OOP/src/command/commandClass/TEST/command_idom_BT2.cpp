#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_idom.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"
#include "../../../blockQueue/blockqueue.h"
#include "test_data.h"

class commandiDom_fixture2 : public iDomTOOLS_ClassTest
{
public:
    commandiDom_fixture2()
        : test_v({"iDom"}),
          test_command_iDom(std::make_unique<command_iDom>("iDom"))
    {
        test_context.main_RFLink = std::make_shared<RFLinkHandler>(&test_context);
        test_context.main_Rs232  = std::make_unique<SerialPi>("test");
    }

protected:
    std::vector<std::string> test_v;
    std::unique_ptr<command_iDom> test_command_iDom;
};

// ──────────────────────────────────────────────────────────────────────────────
// temperature stats – brakujący 4-ty token → out-of-range wcześniej łapane
// przez istniejący test; tu sprawdzamy gałąź "stats" z prawidłową nazwą
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, temperature_stats_not_found)
{
    test_v = {"iDom", "temperature", "stats", "nieistniejacy"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("not found"));
}

// ──────────────────────────────────────────────────────────────────────────────
// 230V
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, power230V_ON)
{
    test_v = {"iDom", "230V", "ON"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "230V ON");
}

TEST_F(commandiDom_fixture2, power230V_OFF)
{
    test_v = {"iDom", "230V", "OFF"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "230V OFF");
}

TEST_F(commandiDom_fixture2, power230V_OUTDOOR)
{
    test_v = {"iDom", "230V", "OUTDOOR"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "change outdoor");
}

TEST_F(commandiDom_fixture2, power230V_wrong_param)
{
    test_v = {"iDom", "230V", "FAKE"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "wrong paramiter");
}

// ──────────────────────────────────────────────────────────────────────────────
// isDay
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, isDay_true)
{
    test_v = {"iDom", "isDay"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    // wynik zależy od czasu systemowego – sprawdzamy tylko format
    EXPECT_THAT(ret, testing::AnyOf(
        testing::StrEq("isDay:true"),
        testing::StrEq("isDay:false")));
}

// ──────────────────────────────────────────────────────────────────────────────
// mqtt – za mało parametrów
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, mqtt_not_enough_params)
{
    test_v = {"iDom", "mqtt", "topic"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "not enough parameters");
}

// ──────────────────────────────────────────────────────────────────────────────
// mqtt – publish
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, mqtt_publish)
{
    test_v = {"iDom", "mqtt", "test/topic", "test_payload"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("done"));
}

// ──────────────────────────────────────────────────────────────────────────────
// weather – za mało parametrów
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, weather_not_enough_params)
{
    test_v = {"iDom", "weather", "krakow"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "not enough parameters");
}

// ──────────────────────────────────────────────────────────────────────────────
// viber – send ok
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, viber_send_ok)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    // mock sendViberMsgBool zwraca STATE::SEND_OK
    auto mockTools = std::make_shared<iDomToolsMock>();
    EXPECT_CALL(*mockTools.get(), sendViberMsgBool(testing::_, testing::_, testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(STATE::SEND_OK));

    thread_context ctx2;
    CONFIG_JSON cfg2;
    cfg2._fb_viber.viberReceiver = {"R1", "R2"};
    cfg2._fb_viber.viberSender   = "sender";
    ctx2.server_settings = &cfg2;
    ctx2.main_iDomTools  = mockTools;
    ctx2.mqttHandler     = std::make_unique<MQTT_mosquitto>("test");

    std::vector<std::string> v2 = {"iDom", "viber", "hello", "world"};
    command_iDom cmd("iDom");
    auto ret = cmd.execute(v2, &ctx2);
    EXPECT_STREQ(ret.c_str(), "wiadomosc wyslana poprawnie");
}

// ──────────────────────────────────────────────────────────────────────────────
// viber – błąd wysyłania
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, viber_send_fail)
{
    auto mockTools = std::make_shared<iDomToolsMock>();
    EXPECT_CALL(*mockTools.get(), sendViberMsgBool(testing::_, testing::_, testing::_, testing::_, testing::_))
        .WillOnce(testing::Return(STATE::ERROR));

    thread_context ctx2;
    CONFIG_JSON cfg2;
    cfg2._fb_viber.viberReceiver = {"R1"};
    cfg2._fb_viber.viberSender   = "sender";
    ctx2.server_settings = &cfg2;
    ctx2.main_iDomTools  = mockTools;
    ctx2.mqttHandler     = std::make_unique<MQTT_mosquitto>("test");

    std::vector<std::string> v2 = {"iDom", "viber", "wiadomosc"};
    command_iDom cmd("iDom");
    auto ret = cmd.execute(v2, &ctx2);
    EXPECT_STREQ(ret.c_str(), "blad wysylania wiadomosci - sprawdz logi");
}

// ──────────────────────────────────────────────────────────────────────────────
// alarm – brak parametru
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, alarm_not_enough_params)
{
    test_v = {"iDom", "alarm"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "not enough parameters");
}

// ──────────────────────────────────────────────────────────────────────────────
// alarm GET
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, alarm_GET)
{
    test_v = {"iDom", "alarm", "GET"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    // zwraca string czasu w formacie HH:MM
    EXPECT_FALSE(ret.empty());
}

// ──────────────────────────────────────────────────────────────────────────────
// alarm OFF
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, alarm_OFF)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    test_context.alarmTime.state = STATE::ACTIVE;
    test_v = {"iDom", "alarm", "OFF"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "alarm clock has been deactivated");
    EXPECT_EQ(test_context.alarmTime.state, STATE::DEACTIVE);
}

// ──────────────────────────────────────────────────────────────────────────────
// alarm ON
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, alarm_ON)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    test_v = {"iDom", "alarm", "ON", "07:30"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "alarm clock has been activated");
    EXPECT_EQ(test_context.alarmTime.state, STATE::ACTIVE);
}

// ──────────────────────────────────────────────────────────────────────────────
// alarm SET from / to / radio
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, alarm_SET_from)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    test_v = {"iDom", "alarm", "SET", "from", "10"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("From Value: 10"));
    EXPECT_EQ(test_context.alarmTime.fromVolume, 10);
}

TEST_F(commandiDom_fixture2, alarm_SET_to)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    test_v = {"iDom", "alarm", "SET", "to", "80"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("To Value: 80"));
    EXPECT_EQ(test_context.alarmTime.toVolume, 80);
}

TEST_F(commandiDom_fixture2, alarm_SET_radio)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    test_v = {"iDom", "alarm", "SET", "radio", "3"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("Radio ID: 3"));
    EXPECT_EQ(test_context.alarmTime.radioID, 3);
}

// ──────────────────────────────────────────────────────────────────────────────
// doorbell – satel nullptr (gałąź else w doorbell)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, doorbell_no_satel)
{
    test_context.satelIntegraHandler = nullptr;
    test_v = {"iDom", "doorbell"};
    // nie powinno rzucić wyjątku
    EXPECT_NO_THROW(test_command_iDom->execute(test_v, &test_context));
}

// ──────────────────────────────────────────────────────────────────────────────
// health check
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(commandiDom_fixture2, health_check)
{
    EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).Times(testing::AnyNumber());
    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).Times(testing::AnyNumber());

    test_v = {"iDom", "health", "check"};
    auto ret = test_command_iDom->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "iDom server health checked");
}
