#pragma once

#include "db-client-if.hpp"

class dbClient : public dbClientIf
{
public:
	dbClient() = default;
	~dbClient() = default;
	HttpStatus::Code upload_iDomData(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &iDomData, uint64_t timestamp, DATABASE* config);
	HttpStatus::Code uploadBulbData(const std::string &name, bool state, std::optional<uint64_t> timestamp = std::nullopt, DATABASE* config = std::nullptr_t());
};