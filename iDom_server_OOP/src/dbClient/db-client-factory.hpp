#pragma once
#include "db-client-if.hpp"
#include "db-client.hpp"

#include <memory>

class dbClientFactory
{
public:
	static std::unique_ptr<dbClientIf> m_db;

	std::unique_ptr<dbClientIf> createDbClient();
};