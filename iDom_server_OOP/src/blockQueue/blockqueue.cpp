#include "blockqueue.h"
#include "../iDom_server_OOP.h"

blockQueue::blockQueue()
{
}
std::mutex blockQueue::mutex_queue_char;
std::queue <MPD_COMMAND> blockQueue::_MPD_CommandQ;
void blockQueue::_add(MPD_COMMAND X)
{
    std::lock_guard <std::mutex>  lock (mutex_queue_char);
    if(_MPD_CommandQ.size() < 10)
    {
        _MPD_CommandQ.push(X);
    }
    else
    {
#ifdef BT_TEST
        std::string e = "za duzo w kolejce";
        throw e;
#else
        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "za dużo danych w kolejce- nie dodaje "<< std::endl;
        log_file_mutex.mutex_unlock();
#endif
    }
}

MPD_COMMAND blockQueue::_get( )
{
    MPD_COMMAND temp = MPD_COMMAND::NULL_;
    std::lock_guard <std::mutex>  lock (mutex_queue_char);
    if (_MPD_CommandQ.empty() == false){
        temp = _MPD_CommandQ.front();
        _MPD_CommandQ.pop();
    }
    return temp;
}

int blockQueue::_size()
{
    std::lock_guard <std::mutex>  lock (mutex_queue_char);
    return _MPD_CommandQ.size();
}

void blockQueue::_clearAll()
{
     std::lock_guard <std::mutex>  lock (mutex_queue_char);
     while (_MPD_CommandQ.empty() == false){
         _MPD_CommandQ.pop();
     }
}
