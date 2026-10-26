#include "../functions.h"
#include "test_data.h"

int useful_F::runLinuxCommand(const std::string& cmd){
    std::cout << "runLinuxCommand(): " << cmd << std::endl;
    return TEST_DATA::runLinuxCommandReturn;
}

// go_while and workServer are defined as inline static std::atomic<bool> in functions.h
