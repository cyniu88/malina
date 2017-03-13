#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <stdlib.h>
#include <queue>
#include <pthread.h>
#include <iostream>
#include <mutex>
#include "../logger/logger.hpp"

enum class MPD_COMMAND {
    PLAY,
    PLAY_ID,
    PAUSE,
    STOP,
    NEXT,
    PREV,
    VOLUP,
    VOLDOWN,
    VOLSET,
    REPEATE,
    RANDOM,
    DEBUG,
    NULL_
};

class blockQueue
{
public:
    blockQueue();
    static std::mutex mutex_queue_char ;

    static std::queue < MPD_COMMAND > _MPD_CommandQ;

    void _add(MPD_COMMAND X);
    MPD_COMMAND _get();
    int  _size();
};

#endif // BLOCQUEUE_H
