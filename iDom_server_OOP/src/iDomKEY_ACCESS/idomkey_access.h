#pragma once

#include "../idom_api.h"
#include "json.hpp"

class iDomKEY_ACCESS : public iDom_API
{
    nlohmann::json m_data;
    std::string m_pathDatabase;
    generator m_generator;
    iDomKEY_ACCESS();
    void writeJSON();
    void readJSON();

public:
    explicit iDomKEY_ACCESS(const std::string &path_database);
    iDomKEY_ACCESS(const iDomKEY_ACCESS &k);
    ~iDomKEY_ACCESS();
    void addKEY(const std::string &name, const std::string &command, size_t size, bool temp = false);
    void addTempKEY(const std::string &name, const std::string &command, size_t size);
    void removeKEY(const std::string &name);
    std::string getKEY(const std::string &name) const;
    std::string listKEY();
    std::string getCommand(const std::string &name) const;
    bool useKEY(const std::string &name, const std::string &key);
    void removeExpiredKeys(unsigned int hours);
    std::string dump() const override;
};
