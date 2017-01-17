#include "blockqueue.h"

blockQueue::blockQueue()
{
}
std::mutex blockQueue::mutex_queue_char;
std::queue < char > blockQueue::_charQ;
void blockQueue::_add(char X)
{
    std::lock_guard <std::mutex>  lock (mutex_queue_char);
    _charQ.push(X);
}

char blockQueue::_get( )
{
    char temp;
    std::lock_guard <std::mutex>  lock (mutex_queue_char);
    if (_charQ.empty() == false){
        temp = _charQ.front();
        _charQ.pop();
    }
    else{
        return 'a';
    }
    return temp;
}

int blockQueue::_size()
{
    std::lock_guard <std::mutex>  lock (mutex_queue_char);
    return _charQ.size();
}
