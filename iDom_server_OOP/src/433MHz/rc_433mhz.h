#ifndef RC_433MHZ_H
#define RC_433MHZ_H
#include "../functions/functions.h"

class RC_433MHz
{
    thread_data * m_my_data;
public:
    RC_433MHz(thread_data * my_data);
    void sendCode(std::string code);
    void receiveCode(std::string code);
};

#endif // RC_433MHZ_H
