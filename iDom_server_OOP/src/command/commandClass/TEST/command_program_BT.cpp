#include "../command_program.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"

class command_program_Class_fixture : public iDomTOOLS_ClassTest
{
public:
    command_program_Class_fixture()
    {

    }

protected:
    std::unique_ptr<command_program> test_command_program;

    std::vector<std::string> test_v;
    void SetUp() final
    {
        iDomTOOLS_ClassTest::SetUp();
        test_command_program = std::make_unique <command_program> ("program");
    }

    void TearDown() final
    {
        iDomTOOLS_ClassTest::TearDown();
    }
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
    EXPECT_THROW(test_command_program->execute(test_v,&test_my_data), std::string);
}

TEST_F(command_program_Class_fixture, programReloadSoft)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("reload");
    test_v.push_back("soft");
    EXPECT_THROW(test_command_program->execute(test_v,&test_my_data), std::string);
    EXPECT_EQ(test_my_data.iDomProgramState, iDomStateEnum::RELOAD);
}

TEST_F(command_program_Class_fixture, programReloadHard)
{
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("reload");
    test_v.push_back("hard");
    EXPECT_THROW(test_command_program->execute(test_v,&test_my_data), std::string);
    EXPECT_EQ(test_my_data.iDomProgramState, iDomStateEnum::HARD_RELOAD);
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

TEST_F(command_program_Class_fixture, debugeVariableProgram)
{
    RFLinkHandler test_RFLinkkHandler(&test_my_data);

    test_my_data.main_RFLink = &test_RFLinkkHandler;
    test_my_data.main_RFLink->okTime = 777;
    test_my_data.main_RFLink->pingTime = 888;
    test_my_data.server_settings->_server.PORT = 88;
    test_my_data.server_settings->_server.v_delay = 1;
    test_my_data.sleeper = 1;
    test_my_data.now_time = 123;
    test_my_data.start = 12;
    int32_t buf = 1;
    unsigned int who = 0;
    test_my_data.pointer.ptr_buf = &buf;
    test_my_data.pointer.ptr_who = &who;

    test_my_data.server_settings->_rs232.BaudRate = 9600;
    test_my_data.server_settings->_rflink.RFLinkBaudRate = 576000;
    test_v.clear();
    test_v.push_back("program");
    test_v.push_back("debuge");
    test_v.push_back("variable");
    auto ret = test_command_program->execute(test_v,&test_my_data);
    std::cout << ret << std::endl;
    EXPECT_THAT(ret,testing::HasSubstr("END"));
}
