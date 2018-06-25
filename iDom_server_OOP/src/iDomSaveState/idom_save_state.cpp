#include <iostream>     // std::cout
#include <fstream>
#include "idom_save_state.h"

iDom_SAVE_STATE::iDom_SAVE_STATE(std::string path): m_path(path)
{
#ifdef BT_TEST
    puts("iDom_SAVE_STATE::iDom_SAVE_STATE()");
#endif
}

iDom_SAVE_STATE::~iDom_SAVE_STATE()
{
#ifdef BT_TEST
    puts("iDom_SAVE_STATE::~iDom_SAVE_STATE()");
#endif
}

nlohmann::json iDom_SAVE_STATE::read()
{
    std::lock_guard<std::mutex> lGuard(m_mutex);
    // read a JSON file
    std::ifstream i(m_path);
    nlohmann::json j;
    i >> j;

    return j;
}

void iDom_SAVE_STATE::write(const nlohmann::json &jj)
{
    std::lock_guard<std::mutex> lGuard(m_mutex);
    // write prettified JSON to another file
    std::ofstream o(m_path);
    o << std::setw(4) << jj << std::endl;
}
