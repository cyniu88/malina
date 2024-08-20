#pragma once

#include "../functions/functions.h"

class RC_433MHz
{
    thread_context *m_context;

public:
    explicit RC_433MHz(thread_context *context);
    void sendCode(const std::string &code);
    std::string receiveCode();
};
