#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../command_program.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"
#include "../../../433MHz/RFLink/rflinkhandler.h"
#include "test_data.h"

class command_program_fixture2 : public testing::Test
{
public:
    command_program_fixture2()
        : test_command_program(std::make_unique<command_program>("program")),
          main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_context.iDomProgramState = iDomStateEnum::WORKING;
        test_context.main_iDomTools   = main_iDomTools;
        test_context.server_settings  = &test_server_set;
        test_context.main_iDomStatus  = std::make_unique<iDomSTATUS>();
        test_context.ptr_MPD_info     = std::make_unique<MPD_info>();
        test_context.ptr_MPD_info->songList = {"song1", "song2"};
        test_context.mqttHandler      = std::make_unique<MQTT_mosquitto>("test");
        test_context.main_RFLink      = std::make_shared<RFLinkHandler>(&test_context);

        test_server_set._fb_viber.viberReceiver = {"R1", "R2"};
        test_server_set._fb_viber.viberSender   = "sender";
    }

protected:
    std::unique_ptr<command_program> test_command_program;
    std::vector<std::string>         test_v;
    thread_context                   test_context;
    std::shared_ptr<iDomToolsMock>   main_iDomTools;
    CONFIG_JSON                      test_server_set;
};

// ──────────────────────────────────────────────────────────────────────────────
// debuge variable – pokrywa 109 niepokrytych linii bloku dump
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_program_fixture2, debuge_variable_contains_context_fields)
{
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
    test_v = {"program", "debuge", "variable"};
    auto ret = test_command_program->execute(test_v, &test_context);
    EXPECT_THAT(ret, testing::HasSubstr("viberReceiver"));
    EXPECT_THAT(ret, testing::HasSubstr("R1"));
}

TEST_F(command_program_fixture2, debuge_variable_contains_mpd_info)
{
    test_context.ptr_MPD_info->artist = "test artist";
    test_context.ptr_MPD_info->radio  = "test radio";
    test_context.ptr_MPD_info->volume = 42;

    test_v = {"program", "debuge", "variable"};
    auto ret = test_command_program->execute(test_v, &test_context);

    EXPECT_THAT(ret, testing::HasSubstr("artist"));
    EXPECT_THAT(ret, testing::HasSubstr("volume"));
    EXPECT_THAT(ret, testing::HasSubstr("42"));
}
