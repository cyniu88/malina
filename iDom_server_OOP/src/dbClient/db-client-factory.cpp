#include "db-client-factory.hpp"

std::unique_ptr<dbClientIf> dbClientFactory::createDbClient()
{
    if (m_db == nullptr)
        return std::make_unique<dbClient>();
    else
        return std::move(m_db);
}

std::unique_ptr<dbClientIf> dbClientFactory::m_db = nullptr;