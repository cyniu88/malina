#include <iostream>

#include "c_connection.h"

std::string C_connection::c_read_buf(int recvSize)
{
    std::string str_buf;
    for (int i = 0; i < recvSize; ++i)
    {
        str_buf.push_back(c_buffer[i]);
    }
    crypto(str_buf, m_encriptionKey, m_encrypted);
    return str_buf;
}
