#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <stdlib.h>
#include <queue>
#include <pthread.h>
#include <iostream>
#include <mutex>

enum class MPD_COMMAND
{
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
    static std::mutex mutex_queue_char;

    static std::queue<MPD_COMMAND> _MPD_CommandQ;

    void _add(MPD_COMMAND X);
    MPD_COMMAND _get() const;
    int _size();
    void _clearAll();
};

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SharedQueue
{
public:
    SharedQueue();
    ~SharedQueue();

    T &front();
    void pop_front();

    void push_back(const T &item);
    void push_back(T &&item);

    int size() const;
    bool empty() const;

private:
    std::deque<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};

template <typename T>
SharedQueue<T>::SharedQueue() {}

template <typename T>
SharedQueue<T>::~SharedQueue() {}

template <typename T>
T &SharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }
    return queue_.front();
}

template <typename T>
void SharedQueue<T>::pop_front()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }
    queue_.pop_front();
}

template <typename T>
void SharedQueue<T>::push_back(const T &item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread
}

template <typename T>
void SharedQueue<T>::push_back(T &&item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(std::move(item));
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread
}

template <typename T>
int SharedQueue<T>::size() const
{
    std::unique_lock<std::mutex> mlock(mutex_);
    int size = queue_.size();
    mlock.unlock();
    return size;
}

template <typename T>
bool SharedQueue<T>::empty() const
{
    std::unique_lock<std::mutex> mlock(mutex_);
    return queue_.empty();
}
#endif // BLOCQUEUE_H
