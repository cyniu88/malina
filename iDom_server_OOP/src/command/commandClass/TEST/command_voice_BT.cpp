#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_voice.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"
#include "../../../HOUSE/TEST/house_room_handler_stub.h"

class command_voice_fixture : public testing::Test
{
public:
    command_voice_fixture()
        : test_command_voice(std::make_unique<command_voice>("voice")),
          main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.main_iDomTools = main_iDomTools;
        test_context.main_house_room_handler =
            std::make_shared<house_room_handler_stub>(&test_context);
        test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("test");
        test_context.main_iDomStatus = std::make_unique<iDomSTATUS>();
    }

protected:
    std::unique_ptr<command_voice>  test_command_voice;
    std::vector<std::string>        test_v;
    thread_context                  test_context;
    std::shared_ptr<iDomToolsMock>  main_iDomTools;
};

// ──────────────────────────────────────────────────────────────────────────────
// włącz radio
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_on_radio)
{
    EXPECT_CALL(*main_iDomTools.get(), MPD_play(testing::_));
    test_v = {"włącz", "radio"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
}

// ──────────────────────────────────────────────────────────────────────────────
// wyłącz radio
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_off_radio)
{
    EXPECT_CALL(*main_iDomTools.get(), MPD_stop());
    test_v = {"wyłącz", "radio"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
}

// ──────────────────────────────────────────────────────────────────────────────
// włącz kodi
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_on_kodi)
{
    EXPECT_CALL(*main_iDomTools.get(), startKodi_Thread())
        .WillOnce(testing::Return("kodi started"));
    test_v = {"włącz", "kodi"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "kodi run");
}

// ──────────────────────────────────────────────────────────────────────────────
// włącz led
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_on_led)
{
    EXPECT_CALL(*main_iDomTools.get(), ledOn())
        .WillOnce(testing::Return("led on"));
    test_v = {"włącz", "led"};
    // ledOn() wywoływane ale brak return w gałęzi – wpada do końca funkcji
    EXPECT_NO_THROW(test_command_voice->execute(test_v, &test_context));
}

// ──────────────────────────────────────────────────────────────────────────────
// wyłącz led
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_off_led)
{
    EXPECT_CALL(*main_iDomTools.get(), ledOFF())
        .WillOnce(testing::Return("led off"));
    test_v = {"wyłącz", "led"};
    EXPECT_NO_THROW(test_command_voice->execute(test_v, &test_context));
}

// ──────────────────────────────────────────────────────────────────────────────
// wyłącz wszystkie światła (dom)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_off_all_lights_dom)
{
    test_v = {"wyłącz", "wszystkie", "światła", "dom"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
}

// ──────────────────────────────────────────────────────────────────────────────
// wyłącz wszystkie światła (wsz)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, turn_off_all_lights_wsz)
{
    test_v = {"zgaś", "wszystko", "światła"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
}

// ──────────────────────────────────────────────────────────────────────────────
// nieznana komenda – zapisuje do pliku i zwraca unknown
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, unknown_command)
{
    test_v = {"nieznana_komenda_xyz"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("unknown command"));
}

// ──────────────────────────────────────────────────────────────────────────────
// pomru – gałąź "pomru" (puste done)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, pomruk)
{
    test_v = {"pomrucz", "coś"};
    auto ret = test_command_voice->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
}

// ──────────────────────────────────────────────────────────────────────────────
// help
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_voice_fixture, help)
{
    auto h = test_command_voice->help();
    EXPECT_FALSE(h.empty());
}
