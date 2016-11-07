
#include "c_connection.h"
#include <iostream>


std::string C_connection::c_read_buf ( ){

    for(char n : c_buffer){
        if (n!=','){

            str_buf+=n;
        }
    }
    return str_buf;
}








