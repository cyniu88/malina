#include "tasker.h"

TASKER::TASKER(thread_data *my_data)
{
    this->my_data = my_data;
}

void TASKER::runTasker()
{
    my_data->mainLCD->checkState();
}
