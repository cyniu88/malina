#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "/home/pi/programowanie/iDom_server_OOP/libs/useful/useful.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest( &argc, argv );

    //::testing::GTEST_FLAG(filter) = "iDomTOOLS_Class.hasTemperatureChange";
<<<<<<< HEAD
    //::testing::GTEST_FLAG(filter) = "iDomTOOLS_Class.button433MHzPressedAction_lockerUnlock";
=======
    ::testing::GTEST_FLAG(filter) = "iDomTOOLS_Class.buttonPressed";
>>>>>>> a8b17aac18706a1e139b5c58d7ec97061b72daf9

    return RUN_ALL_TESTS();
}

