#ifndef C_IRDA_LOGIC_H
#define C_IRDA_LOGIC_H

#include <pthread.h>
#include <queue>
#include <iostream>
#include <wiringPi.h>
#include <string.h>
#include "../blockQueue/blockqueue.h"   // brak
#include "../files_tree/files_tree.h"   //brak
#include "../c_connection/c_connection.h"
#include "../functions/functions.h"

class c_irda_logic
{
private:
    std::queue <char> irda_queue;
    blockQueue char_queue;
    char who;
    thread_data *my_data_logic;

public:
    c_irda_logic(thread_data *my_data);
    void _add(char X);
    char _get();
    int  _size()  const ;
};

#endif // C_IRDA_LOGIC_H
