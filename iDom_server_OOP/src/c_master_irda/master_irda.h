#ifndef MASTER_IRDA_H
#define MASTER_IRDA_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>
#include <map>
#include "../c_connection/c_connection.h"
#include "../KEY/key.h"
#include "../iDom_server_OOP.h"

class master_irda
{
public:
    struct lirc_config *config;

    //Timer for our buttons
    int buttonTimer ;
    int buttonMENU;
   //blockQueue char_queue;

    char *code;
    master_irda(thread_data  *my_data);

    void run();
private:

    void setup();

   thread_data  *my_data2;
};

#endif // MASTER_IRDA_H
