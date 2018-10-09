#include <gtest/gtest.h>
#include <curl/curl.h>
#include "iDomTools_fixture.h"

class iDom_server_Class : public iDomTOOLS_ClassTest
{
protected:
    TEST_JSON test_Json;
    LIGHTNING test_lightning;
    CARDINAL_DIRECTIONS::ALARM_INFO test_struct;
    virtual void SetUp() final
    {
        std::cout << "konfiguracja przed testem iDom_server_Class " <<std::endl;
    }

    virtual void TearDown() final
    {
        std::cout << "czyszczenie po tescie iDom_server_Class " <<std::endl;
    }
};

TEST_F(iDom_server_Class, LED_Strip )
{
    LED_Strip test_LED("1","60","255","255","255", "WHITE");
    EXPECT_STREQ(test_LED.getColorName().c_str(), "WHITE");
    EXPECT_STREQ(test_LED.get(0,60).c_str(),"LED:[1-60-255-255-255];");
    EXPECT_STREQ(test_LED.makeCommand("1","60","128","128","128").c_str(),"LED:[1-60-128-128-128];");

    test_LED.set("1","60","0","255","0","UNKNOWN");

    EXPECT_STREQ(test_LED.getColorName().c_str(), "UNKNOWN");
    EXPECT_STREQ(test_LED.get(0,60).c_str(),"LED:[1-60-0-255-0];");

    test_LED.set(50,51,1,2,3,"BLACK");

    EXPECT_STREQ(test_LED.getColorName().c_str(), "BLACK");
    EXPECT_STREQ(test_LED.get(0,60).c_str(),"LED:[50-51-1-2-3];");
}
