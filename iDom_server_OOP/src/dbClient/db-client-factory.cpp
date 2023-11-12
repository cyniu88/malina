#include "db-client-factory.hpp"

std::unique_ptr<dbClientIf> dbClientFactory::createDbClient()
{
    if (m_db == std::nullptr_t())
        return std::make_unique<dbClient>();
    else
        return std::move(m_db);
}

std::unique_ptr<dbClientIf> dbClientFactory::m_db = std::nullptr_t();