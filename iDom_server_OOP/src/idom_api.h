#ifndef IDOM_API_H
#define IDOM_API_H

#include <map>

class iDom_API
{
public:
    static std::map<std::string, iDom_API*> m_map_iDom_API;
    virtual ~iDom_API(){};
    virtual std::string dump() const = 0;
    virtual void addToMap(const std::string& , iDom_API*) = 0; // add to constructor
    virtual void removeFromMap(const std::string&) = 0; // add to destructor

    static std::string getDump();
};


#endif // IDOM_API_H
