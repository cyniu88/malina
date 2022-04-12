#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../iDomTools/test/iDomTools_fixture.h"
#include "../thermometer_container.h"

class Thermometer_container_fixture :  public ::testing::Test
{
public:
    THERMOMETER_CONTAINER testThermo;
    std::string termoName = "test_thermometer";
    std::vector<std::string> v = {"10.2","11.22"};
    Thermometer_container_fixture()
    {
        testThermo.add("inside");
        testThermo.add("outside");
        testThermo.updateAll(&v);
        v = {"20.2","21.22"};
        testThermo.updateAll(&v);
        std::cout << "temepratura inside" << testThermo.getTemp("inside") << std::endl;
        std::cout << "temepratura outside" << testThermo.getTemp("outside") << std::endl;
        testThermo.updateStats("inside");
        testThermo.updateStats("outside");
        std::cout << "rozmiar mapy termoetrow: " << testThermo.sizeOf() << std::endl;
        testThermo.showAll();
    }
};
TEST_F(Thermometer_container_fixture, returnUnexistPTR)
{
    EXPECT_THROW(testThermo.returnThermometerPtr("fake"),std::string);
}

TEST_F(Thermometer_container_fixture, getStatsByName)
{
    std::string returnedStr = testThermo.getStatsByName("inside");
    std::cout << "zwrocono " << returnedStr << std::endl;
    EXPECT_THAT(returnedStr, testing::HasSubstr("min: 20.2"));
    EXPECT_THAT(returnedStr, testing::HasSubstr("max: 20.2"));
}

TEST_F(Thermometer_container_fixture, getLast2)
{
    std::cout << "rozmiar mapy termoetrow: " << testThermo.sizeOf() << std::endl;
    testThermo.updateStats("inside");
    testThermo.updateStats("outside");
    testThermo.showAll();
    v = {"44.4","45.45"};
    testThermo.updateAll(&v);
    testThermo.updateStats("inside");
    testThermo.updateStats("outside");
    std::string returnedStr = testThermo.getStatsByName("inside");
    std::cout << "zwrocono " << returnedStr <<"or"<< std::endl;
    auto v = testThermo.getLast2("inside");
    EXPECT_EQ(v.first, 20.2);
    EXPECT_EQ(v.second, 44.4);
}

TEST_F(Thermometer_container_fixture, isMoreDiff)
{
    std::cout << "rozmiar mapy termoetrow: " << testThermo.sizeOf() << std::endl;
    testThermo.updateStats("inside");
    testThermo.updateStats("outside");
    testThermo.showAll();
    v = {"24.4","45.45"};
    testThermo.updateAll(&v);
    testThermo.updateStats("inside");
    testThermo.updateStats("outside");
    EXPECT_FALSE(testThermo.isMoreDiff("inside",15.5));
    EXPECT_TRUE(testThermo.isMoreDiff("outside",15.5));
}
