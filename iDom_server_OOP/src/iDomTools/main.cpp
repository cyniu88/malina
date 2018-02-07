#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "idomtools_useful.h"
#include "idomtools.h"
#include "../../libs/useful/useful.h"



  TEST(ClockClass, clock_obj) {
      Clock f(13,57);
      Clock g(23,59);
      Clock r = f+g;
      EXPECT_EQ(r.getString(), "13:56");
      EXPECT_EQ(f<g, true);
      r+=Clock("04:04");
      EXPECT_EQ(r.getString(),"18:00");
  }

  TEST(idomTOOLsClass, tc_hasTemperatureChange) {

      //iDomTOOLS::hasTemperatureChange
      iDomTOOLS test_iDomTOOLS(NULL);


      EXPECT_EQ(1,1);
  }

  int main(int argc, char **argv) {
      ::testing::InitGoogleTest( &argc, argv );
      return RUN_ALL_TESTS();
  }
