
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

void C_connection::l_send_file(std::string path, std::string find  , bool reverse )
{
    if(find.empty()==true)
    {
        std::fstream log_file;
        log_file.open(path.c_str(),std::ios::in);
        if( log_file.good() == false )
        {
            str_buf =" can not open file !";

            return;
        }
        else
        {
            str_buf.erase();
            while( !log_file.eof() )
            {
                str_buf += log_file.get();
            }
            str_buf.erase(str_buf.length()-1, str_buf.length());

            log_file.close();
        }
    }
    else
    {
        std::fstream log_file;
        log_file.open(path.c_str(),std::ios::in);
        if( log_file.good() == false )
        {
            str_buf =" can not open file !";

            return;
        }
        else
        {
            str_buf.erase();
            std::string str_temp;
            while( std::getline(log_file,str_temp) )
            {

                if (reverse){
                    if(std::string::npos!=str_temp.find(find)){
                        str_buf+=str_temp +"\n";

                    }
                    else{
                        if(str_buf.size()<3){
                            str_buf+="    ";
                        }
                    }

                }
                else{
                    if(std::string::npos == str_temp.find(find)){
                        str_buf+=str_temp+"\n";


                    }
                }
                if(str_buf.size()<3){
                    str_buf+="    ";
                }
            }

        }
        log_file.close();
    }
}







