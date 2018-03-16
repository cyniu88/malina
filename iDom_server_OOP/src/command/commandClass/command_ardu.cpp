#include "command_ardu.h"

command_ardu::command_ardu(std::string name):command(name)
{

}

std::string command_ardu::execute(std::vector<std::string> &v, thread_data *my_data)
{
//    puts("command ardu");
//    for (auto str : v)
//    {puts(str.c_str());}
//std::cout << "w ardu jest " << v[1]<<std::endl;
    std::string str_buf = " only for internal usage!";
    if (v.size() > 2){
        std::string msg;
        for (unsigned int i = 2 ; i < v.size(); ++i){
            msg+=" ";
            msg+=v[i];
        }

        if(v[1] == "433MHz"){
            //my_data->myEventHandler.run("433MHz")->addEvent("odebrano ID 433MHz : "+msg);
            try{
               my_data->main_iDomTools->button433MHzPressedAction(
                            my_data->main_iDomTools->buttonPressed(v[2])
                        );
            }
           catch(std::string e){
               // my_data->myEventHandler.run("433MHz")->addEvent("odebrano ID 433MHz "+msg);
            }
        }
    }
    return str_buf;
}

std::string command_ardu::help()
{
    return " only for internal usege\n";
}
