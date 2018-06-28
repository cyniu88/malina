#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gconv.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );

    //::testing::GTEST_FLAG(filter) = "functions_fixture.sleepThread";
    //::testing::GTEST_FLAG(filter) = "Switch_Class.switch_alarm_on";
    //::testing::GTEST_FLAG(filter) = "iDomTOOLS_ClassTest.testCPU_Load";
    return RUN_ALL_TESTS();
}

