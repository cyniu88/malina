#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "/home/pi/programowanie/iDom_server_OOP/libs/useful/useful.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );

    //::testing::GTEST_FLAG(filter) = "iDomTOOLS_Class.buttonPressed";
    //::testing::GTEST_FLAG(filter) = "RFLinkHandler_Class.getValue";
    return RUN_ALL_TESTS();
}
