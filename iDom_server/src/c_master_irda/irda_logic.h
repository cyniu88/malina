#ifndef IRDA_LOGIC_H
#define IRDA_LOGIC_H
#include <queue>
#include <iostream>
#include <string.h>
#include "../blockQueue/blockqueue.h"
#include "../files_tree/files_tree.h"
#include "../wiadomosc/wiadomosc.h"
//#include "../iDom_server.h"
extern bool go_while;
class irda_logic
{
private:
    std::queue <char> irda_queue;
    blockQueue char_queue;
    char who;


public:
    irda_logic();
    void _add(char X);
    char _get();
    int  _size();
    void _distribute();

};

#endif // IRDA_LOGIC_H
