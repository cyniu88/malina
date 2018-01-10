#include "rc_433mhz.h"

RC_433MHz::RC_433MHz(thread_data *my_data)
{
    this->m_my_data = my_data;
    puts("wyszedl w RC_433MHz::RC_433MHz");
    std::cout <<  this->m_my_data<< std::endl;
}

void RC_433MHz::sendCode(std::string code)
{
    puts("nispokoziomowo wyslalem kod");
    std::cout <<code <<std::endl;
    puts("wyszedl w sendCode");
    std::cout <<  this->m_my_data<< std::endl;
    puts("|");
    std::string recv = useful_F::send_to_arduino(m_my_data,"433MHz:"+ code +";");
}

void RC_433MHz::receiveCode(std::string code)
{
    puts("code reveived but nothing to do ");
}
