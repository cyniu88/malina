#ifndef IDOM_API_H
#define IDOM_API_H

#include <map>
#include <mutex>

#include "../libs/generator/generator.h"

class iDom_API
{
public:
    std::string m_className;
    static std::mutex m_locker;
    static std::map<std::string, iDom_API*> m_map_iDom_API;
    iDom_API(){
        generator g;
        m_className = "\n";
        m_className.append(g.random_string(10));
       // m_className.append(" \n");
    }
    virtual ~iDom_API() = default;
    virtual std::string dump() const = 0;

    static void addToMap(const std::string& , iDom_API*); // add to constructor
    static void removeFromMap(const std::string&); // add to destructor

    static std::string getDump();
};


#endif // IDOM_API_H
