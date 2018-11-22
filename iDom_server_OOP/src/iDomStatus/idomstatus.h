#ifndef IDOMSTATUS_H
#define IDOMSTATUS_H
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include "../../libs/useful/useful.h"

class iDomSTATUS
{
    std::map<std::string, STATE> m_stateMAP;
    std::mutex m_lockGuard;
public:
    iDomSTATUS();
    void addObject(std::string name, STATE st = STATE::UNDEFINE );
    void setObjectState(const std::string &name, STATE st);
    STATE getObjectState(const std::string &name);
    std::string getObjectStateString(const std::string &name);
    std::string getAllObjectsStateString();
};

#endif // IDOMSTATUS_H
