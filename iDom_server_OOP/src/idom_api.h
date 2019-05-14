#ifndef IDOM_API_H
#define IDOM_API_H

#include <map>
#include <mutex>

class iDom_API
{
public:
    static std::mutex m_locker;
    static std::map<std::string, iDom_API*> m_map_iDom_API;
    virtual ~iDom_API(){};
    virtual std::string dump() const = 0;

    static void addToMap(const std::string& , iDom_API*); // add to constructor
    static void removeFromMap(const std::string&); // add to destructor

    static std::string getDump();
};


#endif // IDOM_API_H
