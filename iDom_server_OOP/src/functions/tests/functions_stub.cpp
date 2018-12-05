#include "../functions.h"
#include "test_data.h"

void useful_F::clearThreadArray(thread_data *my_data)
{
    std::cout << "useful_F::clearThreadArray(thread_data *my_data)" << std::endl;
}


int useful_F::runLinuxCommand(const std::string& cmd){
    std::cout << "runLinuxCommand(): " << cmd << std::endl;
    return TEST_DATA::runLinuxCommandReturn;
}

bool useful_F::go_while = true;
bool useful_F::workServer = true;

void useful_F::button_interrupt(){}

std::string useful_F::send_to_arduino(thread_data *my_data, const std::string& d){
    puts("useful_F::send_to_arduino()");
    return TEST_DATA::return_send_to_arduino;
}
