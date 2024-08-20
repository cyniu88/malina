#ifndef C_IRDA_LOGIC_H
#define C_IRDA_LOGIC_H

#include <pthread.h>
#include <queue>
#include <iostream>
#include <string.h>
#include "../blockQueue/blockqueue.h" // brak
#include "../iDom_server_OOP.h"

enum class PILOT_STATE{
    MPD,
    LED,
    MENU,
    PROJECTOR,
    TEMPERATURE,
    MOVIE,
    SLEEPER
};

class c_irda_logic : public iDom_API
{
#ifdef BT_TEST
public:
#endif
    blockQueue mpd_queue;
    PILOT_STATE who;
    thread_context *context;
    void irdaMPD(PILOT_KEY X);
    void sleeperLogic(PILOT_KEY X);
    void projectorLogic(PILOT_KEY X);
    void movieLogic(PILOT_KEY X);
    void menuLogic(PILOT_KEY X);
    void mainPilotHandler(PILOT_KEY X);

public:
    explicit c_irda_logic(thread_context *context);
    ~c_irda_logic() override;
    void _add(PILOT_KEY X);

    ///////////// iDom API ///////////////
    std::string dump() const override;
};

#endif // C_IRDA_LOGIC_H
