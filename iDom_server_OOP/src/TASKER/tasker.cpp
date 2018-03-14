#include "tasker.h"
#include "../functions/functions.h"

TASKER::TASKER(thread_data *my_data): rs232(my_data)
{
    this->my_data = my_data;
}

void TASKER::runTasker()
{
    my_data->mainLCD->checkState();
}

void TASKER::dataFromRS232(std::string bufor)
{
   // std::cout <<" TASKER::dataFromRS232()- " << bufor << " " << Clock::getTime().toSeconds() <<std::endl;
    std::vector <std::string> v;
    useful_F::tokenizer(v," \n, ", bufor);


    rs232.run(v,my_data);
}
