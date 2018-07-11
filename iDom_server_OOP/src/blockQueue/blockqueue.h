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

template<typename T>
class blockQueue
{
public:
    blockQueue();
    static std::mutex mutex_queue_char ;

    static std::queue <T> _MPD_CommandQ;

    void _add(T X);
    T _get();
    int  _size();
    void _clearAll();
};

#endif // BLOCQUEUE_H
