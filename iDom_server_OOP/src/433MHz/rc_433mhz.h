#ifndef RC_433MHZ_H
#define RC_433MHZ_H
#include "../functions/functions.h"

class RC_433MHz
{
    thread_data *m_my_data;

public:
    explicit RC_433MHz(thread_data *my_data);
    void sendCode(const std::string &code);
    std::string receiveCode();
};

#endif // RC_433MHZ_H
