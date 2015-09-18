#ifndef IRDA_LOGIC_H
#define IRDA_LOGIC_H
#include <queue>
#include <iostream>
#include "../blockQueue/blockqueue.h"

class irda_logic
{
private:
    std::queue <char> irda_queue;
    blockQueue char_queue;
    char who;
    int led_flag;

public:
    irda_logic();
    void _add(char X);
    char _get();
    int  _size();
    void _distribute();

};

#endif // IRDA_LOGIC_H
