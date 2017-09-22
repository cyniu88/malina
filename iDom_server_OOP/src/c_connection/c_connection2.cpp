
#include "c_connection.h"
#include <iostream>


std::string C_connection::c_read_buf ( int  recvSize){

    std::string str_buf;
    for (int i = 0 ; i < recvSize; ++i){
        str_buf.push_back( c_buffer[i]);
    }
    crypto(str_buf,encriptionKey,m_encrypted);
    //puts("po konwersji");
    //puts(str_buf.c_str());
    return str_buf;
}








