#include "tasker.h"
#include "../functions/functions.h"
#include "../command/commandhandlerrs232.h"

TASKER::TASKER(thread_data *my_data)
{
    this->my_data = my_data;
}

void TASKER::runTasker()
{
    my_data->mainLCD->checkState();
}

void TASKER::dataFromRS232(std::string bufor)
{
    std::vector <std::string> v;
    useful_F::tokenizer(v," \n, ", bufor);
    commandHandlerRS232 rs232(my_data);

    rs232.run(v,my_data);
}
