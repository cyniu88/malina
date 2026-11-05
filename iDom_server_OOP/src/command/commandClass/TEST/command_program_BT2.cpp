#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_program.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"
#include "../../../buderus/mock/buderus_mock.h"
#include "../../../functions/functions.h"
#include "test_data.h"

class command_program_fixture2 : public testing::Test
{
public:
    command_program_fixture2()
        : test_command_program(std::make_unique<command_program>("program")),
          main_iDomTools(std::make_shared<iDomToolsMock>()),
          testBuderusMock(std::make_shared<BUDERUS_MOCK>())
    {
        test_context.iDomProgramState = iDomStateEnum::WORKING;
        test_context.main_iDomTools   = main_iDomTools;
        test_context.server_settings  = &test_server_set;
        test_context.main_iDomStatus  = std::make_unique<iDomSTATUS>();
        test_context.ptr_MPD_info     = std::make_unique<MPD_info>();
        test_context.ptr_MPD_info->songList = {"song1", "song2"};
        test_context.mqttHandler      = std::make_unique<MQTT_mosquitto>("test");
        test_context.main_RFLink      = std::make_shared<RFLinkHandler>(&test_context);
        test_context.ptr_buderus      = testBuderusMock;

        test_server_set._fb_viber.viberReceiver = {"R1", "R2"};
        test_server_set._fb_viber.viberSender   = "sender";

        // wymagane przez iDomSTATUS::getAllObjectsStateString() → myStaticCtx->ptr_buderus
        useful_F::myStaticCtx = &test_context;
    }

protected:
    std::unique_ptr<command_program> test_command_program;
    std::vector<std::string>         test_v;
    thread_context                   test_context;
    std::shared_ptr<iDomToolsMock>   main_iDomTools;
    std::shared_ptr<BUDERUS_MOCK>    testBuderusMock;
    CONFIG_JSON                      test_server_set;
};

// ──────────────────────────────────────────────────────────────────────────────
// debuge variable – pokrywa blok dump (109 linii wcześniej niepokrytych)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_program_fixture2, debuge_variable_contains_context_fields)
{
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv())
        .WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*testBuderusMock.get(), getHeatingStartTime())
        .WillRepeatedly(testing::Return(0));

    test_v = {"program", "debuge", "variable"};
    auto ret = test_command_program->execute(test_v, &test_context);

    EXPECT_THAT(ret, testing::HasSubstr("alarmTime"));
    EXPECT_THAT(ret, testing::HasSubstr("serverStarted"));
    EXPECT_THAT(ret, testing::HasSubstr("mqttHandler"));
    EXPECT_THAT(ret, testing::HasSubstr("rflink map size"));
    EXPECT_THAT(ret, testing::HasSubstr("END."));
}

TEST_F(command_program_fixture2, debuge_variable_contains_viber_receiver)
{
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv())
        .WillRepeatedly(testing::Return(false));
    EXPECT_CALL(*testBuderusMock.get(), getHeatingStartTime())
        .WillRepeatedly(testing::Return(0));

    test_v = {"program", "debuge", "variable"};
    auto ret = test_command_program->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("viberReceiver"));
    EXPECT_THAT(ret, testing::HasSubstr("R1"));
}

TEST_F(command_program_fixture2, debuge_variable_contains_mpd_info)
{
    EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv())
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*testBuderusMock.get(), getHeatingStartTime())
        .WillRepeatedly(testing::Return(12345));

    test_context.ptr_MPD_info->artist = "test artist";
    test_context.ptr_MPD_info->radio  = "test radio";
    test_context.ptr_MPD_info->volume = 42;

    test_v = {"program", "debuge", "variable"};
    auto ret = test_command_program->execute(test_v, &test_context);

    EXPECT_THAT(ret, testing::HasSubstr("artist"));
    EXPECT_THAT(ret, testing::HasSubstr("volume"));
    EXPECT_THAT(ret, testing::HasSubstr("42"));
}
