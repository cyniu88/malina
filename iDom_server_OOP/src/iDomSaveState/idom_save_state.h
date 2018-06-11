#ifndef IDOM_SAVE_STATE_H
#define IDOM_SAVE_STATE_H

#include "../iDom_server_OOP.h"
#include "json.hpp"

class iDom_SAVE_STATE
{
public:
    iDom_SAVE_STATE(std::string path);
    ~iDom_SAVE_STATE();

    nlohmann::json read();
    void write(const nlohmann::json &jj);

private:
    std::string m_path = "NULL";
};

#endif // IDOM_SAVE_STATE_H
