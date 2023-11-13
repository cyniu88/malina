#pragma once

#include "db-client-if.hpp"

class dbClient : public dbClientIf
{
public:
	dbClient() = default;
	~dbClient() = default;
	HttpStatus::Code upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &iDomData);
	HttpStatus::Code uploadBulbData(const std::string &name, bool state);
};