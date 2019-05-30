#ifndef IDOMKEY_ACCESS_H
#define IDOMKEY_ACCESS_H

#include "../idom_api.h"
#include "../json2/json.hpp"
#include "../libs/generator/generator.h"

class iDomKEY_ACCESS: public iDom_API
{
    nlohmann::json m_data;
    std::string m_pathDatabase;
    generator m_generator;
    iDomKEY_ACCESS();
    void writeJSON();
    void readJSON();

public:
    iDomKEY_ACCESS(const std::string& path_database);
    iDomKEY_ACCESS(const iDomKEY_ACCESS& k);
    ~iDomKEY_ACCESS();
    void addKEY(const std::string& name, size_t size, bool temp = false);
    void addTempKEY(const std::string& name, size_t size);
    void removeKEY(const std::string& name);
    std::string getKEY(const std::string& name);
    std::string listKEY();
    bool useKEY(const std::string& name, const std::string& key);

    std::string dump() const;
};

#endif // IDOMKEY_ACCESS_H
