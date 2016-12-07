
#include "c_connection.h"
#include <iostream>


std::string C_connection::c_read_buf ( int  recvSize){

    std::string str_buf;
    for (int i = 0 ; i < recvSize; ++i){
        str_buf+= c_buffer[i];
    }
    return str_buf;
}








