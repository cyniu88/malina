#include "commandrs232.h"

#include "../iDom_server_OOP/src/functions/functions.h"

commandRS232::commandRS232(std::string name):command(name)
{

}

std::string commandRS232::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf ="";

        if (v[1]=="get")
        {
            if (v[2]=="temperature"){
                //std::cout << " szukam temeratury" << std::endl;

                str_buf = send_to_arduino(my_data,"temperature:339;");
                str_buf += std::to_string(++counter);

            }
            else
            {
                str_buf = ("wrong parameter: "+v[2]);

            }


        }

        else if (v[1]=="send")
        {
            //std::cout << "!!!!!!!!!!!!!!!!! " << command[2] << std::endl;
            str_buf = send_to_arduino(my_data,v[2]);

        }
        else
        {
            str_buf = ("wrong parameter: "+v[1]);

        }


    return str_buf;
}
