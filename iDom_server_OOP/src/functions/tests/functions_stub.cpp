#include "../functions.h"
#include "test_data.h"

int useful_F::runLinuxCommand(const std::string& cmd){
    std::cout << "runLinuxCommand(): " << cmd << std::endl;
    return TEST_DATA::runLinuxCommandReturn;
}

bool useful_F::go_while = true;
bool useful_F::workServer = true;
