
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
            c_send(0);
            c_recv(0);
            return;
        }
        else
        {
            str_buf.erase();
            while( !log_file.eof() )
            {
                for (int i =0 ; i< MAX_buf ; ++i)
                {
                    str_buf += log_file.get();
                    if (log_file.eof()==1)
                    {
                        str_buf.erase(str_buf.length()-1, str_buf.length());
                        break;
                    }

                }

                std::cout << " wielkosc bufora z send "<< str_buf.length() << std::endl;
                c_send(0);
                 std::cout << "w local FILES send "<<std::endl;
                c_recv(0);


                std::cout << " \npo recv w FILES" <<std::endl;
                str_buf.erase();
                //c_recv(0);
            }
            //c_send("dupa");
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
            c_send(0);
            c_recv(0);
            return;
        }
        else
        {
            str_buf.erase();
            while( std::getline(log_file,str_buf) )
            {

                if (reverse){
                    if(std::string::npos!=str_buf.find(find)){
                        str_buf+="\n";
                        c_send(0);
                        c_recv(0);
                    }
                }
                else{
                    if(std::string::npos ==str_buf.find(find)){
                        str_buf+="\n";
                        if(str_buf.size()<3){
                            str_buf+="    ";
                        }
                        c_send(0);
                        c_recv(0);
                    }
                }
                str_buf.erase();
            }
            log_file.close();
        }
    }
}






