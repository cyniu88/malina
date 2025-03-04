#include <gtest/gtest.h>

#include "../command_recuperator.h"
#include "../../../recuperator/mock/recuperator_mock.h"

class command_recuperator_Class_fixture : public testing::Test
{
public:
    command_recuperator_Class_fixture() : test_command_recuperator(std::make_unique<command_recuperator>("ip"))
    {
        test_context.server_settings = &server_settings;
        test_context.m_recuperator = std::make_unique<RecuperatorMock>();
        mockRecuperator = dynamic_cast<RecuperatorMock*>(test_context.m_recuperator.get());
    }

protected:
    std::unique_ptr<command_recuperator> test_command_recuperator;
    std::vector<std::string> test_v;
    thread_context test_context;
    CONFIG_JSON server_settings;
    RecuperatorMock* mockRecuperator;
};

TEST_F(command_recuperator_Class_fixture, set_away)
{
    test_v.push_back("recuperator");
    test_v.push_back("set");
    test_v.push_back("away");
    EXPECT_CALL(*mockRecuperator, setAwayMode());
    auto ret = test_command_recuperator->execute(test_v, &test_context);
    std::cout << "wynik testu: " << ret << std::endl;
}

TEST_F(command_recuperator_Class_fixture, set_away_time)
{
    test_v.push_back("recuperator");
    test_v.push_back("set");
    test_v.push_back("away");
    test_v.push_back("10");
    test_v.push_back("min");
    EXPECT_CALL(*mockRecuperator, setAwayModeTime(true, 10, "min"));
    auto ret = test_command_recuperator->execute(test_v, &test_context);
    std::cout << "wynik testu: " << ret << std::endl;
}
