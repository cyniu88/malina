#ifndef RC_433MHZ_H
#define RC_433MHZ_H
#include "../functions/functions.h"

class RC_433MHz
{
    thread_data *m_context;

public:
    explicit RC_433MHz(thread_data *context);
    void sendCode(const std::string &code);
    std::string receiveCode();
};

#endif // RC_433MHZ_H
