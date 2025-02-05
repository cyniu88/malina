#pragma once

#include <mutex>
#include "json.hpp"

class iDom_SAVE_STATE
{
public:
    mutable std::mutex m_mutex;
    explicit iDom_SAVE_STATE(const std::string &path);
    ~iDom_SAVE_STATE() = default;

    nlohmann::json read() const;
    void write(const nlohmann::json &jj);

private:
    std::string m_path = "NULL";
};

