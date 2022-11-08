#include<gtest/gtest.h>
#include<gmock/gmock.h>

#include "../command_program.h"
#include "../../../iDomTools/mock/iDomToolsMock.h"

class command_program_Class_fixture : public testing::Test
{
public:
    command_program_Class_fixture(): test_command_program(std::make_unique <command_program> ("program")),
        main_iDomTools(std::make_shared<iDomToolsMock>())
    {
        test_my_data.iDomProgramState = iDomStateEnum::WORKING;
        test_my_data.main_iDomTools = main_iDomTools;
        test_my_data.server_settings = &test_server_set;
    }

protected:
    std::unique_ptr<command_program> test_command_program;
    std::vector<std::string> test_v;
    thread_data test_my_data;
    std::shared_ptr<iDomToolsMock> main_iDomTools;
    CONFIG_JSON test_server_set;
};

TEST_F(command_program_Class_fixture, unknownParameter)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("test");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(), "add more paramiters");

}

TEST_F(command_program_Class_fixture, missingParameter)
{
    test_v.clear();
    test_v.push_back("program");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    EXPECT_THAT(ret, ::testing::HasSubstr("what?"));
}

TEST_F(command_program_Class_fixture, fakeParameter)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("fake");
    test_v.push_back("fake2");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    EXPECT_THAT(ret, ::testing::HasSubstr("what? - fake"));
}

TEST_F(command_program_Class_fixture, stopProgram)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("stop");
    EXPECT_CALL(*main_iDomTools.get(), close_iDomServer());
    test_command_program->execute(test_v,&test_my_data);
}

TEST_F(command_program_Class_fixture, programReloadSoft)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("reload");
    test_v.push_back("soft");
    EXPECT_CALL(*main_iDomTools.get(), reloadSoft_iDomServer());
    test_command_program->execute(test_v,&test_my_data);
}

TEST_F(command_program_Class_fixture, programReloadHard)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("reload");
    test_v.push_back("hard");
    EXPECT_CALL(*main_iDomTools.get(), reloadHard_iDomServer());
    test_command_program->execute(test_v,&test_my_data);
}

TEST_F(command_program_Class_fixture, clearRamProgram)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("clear");
    test_v.push_back("ram");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"ram has beed freed");
}

TEST_F(command_program_Class_fixture, raspberryProgram)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("raspberry");
    test_v.push_back("command");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    EXPECT_STREQ(ret.c_str(),"command done with exitcode: 0");
}

TEST_F(command_program_Class_fixture, version)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct tm *parts = std::localtime(&now_c);


    std::stringstream timeString;
    timeString << parts->tm_mday << " " << (1900 + parts->tm_year);
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("version");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret, testing::HasSubstr(timeString.str()));
}
