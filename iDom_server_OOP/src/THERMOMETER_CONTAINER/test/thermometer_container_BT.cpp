#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../iDomTools/test/iDomTools_fixture.h"
#include "../thermometer_container.h"

class thermometer_container_fixture : public iDomTOOLS_ClassTest
{
public:
   THERMOMETER_CONTAINER testThermo;
   std::string termoName = "test_thermometer";
   void setUp()
   {
       std::vector<std::string> v;
       v.push_back("10");
       v.push_back("10");

       testThermo.add(termoName);
       testThermo.updateAll(&v);
   }
};
TEST_F(thermometer_container_fixture, returnUnexistPTR)
{
    EXPECT_THROW(testThermo.returnThermometerPtr("fake"),std::string);
}

TEST_F(thermometer_container_fixture, updateStats)
{
    std::cout << testThermo.getStatsByName(termoName);
}
