#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../iDomTools/test/iDomTools_fixture.h"
#include "../thermometer_container.h"

class Thermometer_container_fixture :  public ::testing::Test //public iDomTOOLS_ClassTest
{
public:
   THERMOMETER_CONTAINER testThermo;
   std::string termoName = "test_thermometer";
   std::vector<std::string> v;

   void SetUp() final
   {
       v.push_back("10.2");
       v.push_back("10.3");

       testThermo.add(termoName);
       testThermo.add("inside");
       testThermo.add("outside");
       testThermo.updateAll(&v);
       puts("SetUp() Thermometer_container_fixture");

       std::cout << "rozmiar mapy termoetrow: " << testThermo.sizeOf() << std::endl;
    }
};
TEST_F(Thermometer_container_fixture, returnUnexistPTR)
{
    EXPECT_THROW(testThermo.returnThermometerPtr("fake"),std::string);
}

TEST_F(Thermometer_container_fixture, getStatsByName)
{
    std::string returnedStr = testThermo.getStatsByName(termoName);
    std::cout << "zwrocono " << returnedStr << std::endl;
    EXPECT_EQ(1,1);
}

TEST_F(Thermometer_container_fixture, getLast2)
{
    testThermo.updateAll(&v);
    testThermo.updateAll(&v);
   // testThermo.updateStats("inside");
    testThermo.setTemp("inisde",30.9);
    testThermo.setTemp("inisde",32.9);
    std::cout << "rozmiar mapy termoetrow: " << testThermo.sizeOf() << std::endl;

    auto v = testThermo.getLast2("inside");
    EXPECT_EQ(v.first, 10.2);
    EXPECT_EQ(v.second, 10.3);
}

