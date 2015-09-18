#ifndef MASTER_IRDA_H
#define MASTER_IRDA_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>
#include "../wiadomosc/wiadomosc.h"
#include "../functions/functions.h"
#include "../logger/logger.hpp"
#include "../blockQueue/blockqueue.h"
#include "irda_logic.h"

extern Logger log_file_mutex;
extern char * _logfile;

class master_irda
{
public:
    struct lirc_config *config;

    //Timer for our buttons
    int buttonTimer ;
    int buttonMENU;
   // blockQueue char_queue;
    irda_logic irda_queue;
    char *code;
    master_irda(thread_data  *my_data);
    void setup();
    void run();
};

#endif // MASTER_IRDA_H
