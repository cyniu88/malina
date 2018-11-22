#ifndef C_IRDA_LOGIC_H
#define C_IRDA_LOGIC_H

#include <pthread.h>
#include <queue>
#include <iostream>
#include <wiringPi.h>
#include <string.h>
#include "../blockQueue/blockqueue.h" // brak
#include "../files_tree/files_tree.h" //brak
#include "../c_connection/c_connection.h"
#include "../functions/functions.h"

enum class PILOT_STATE{
    MPD,
    LED,
    MENU,
    PROJECTOR,
    TEMPERATURE,
    MOVIE,
    SLEEPER
};

class c_irda_logic
{
#ifdef BT_TEST
public:
#endif
    //std::queue <PILOT_KEY> irda_queue;
    blockQueue mpd_queue;
    PILOT_STATE who;
    thread_data *my_data;
    void irdaMPD(PILOT_KEY X);
    void sleeperLogic(PILOT_KEY X);
    void projectorLogic(PILOT_KEY X);
    void movieLogic(PILOT_KEY X);
    void menuLogic(PILOT_KEY X);
    void mainPilotHandler(PILOT_KEY X);

public:
    c_irda_logic(thread_data *my_data);
    void _add(PILOT_KEY X);
//    PILOT_KEY _get();
//    int _size() const;
};

#endif // C_IRDA_LOGIC_H
