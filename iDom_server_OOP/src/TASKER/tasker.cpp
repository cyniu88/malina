#include "tasker.h"
#include "../functions/functions.h"

TASKER::TASKER(thread_data *my_data)
{
    this->my_data = my_data;
}

void TASKER::runTasker()
{
    if(counter == 0){
        my_data->mainLCD->checkState();
        ++counter;
    }
    else{
        counter = 0;
    }
}
