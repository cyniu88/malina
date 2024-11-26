#pragma once

#include "db-client-if.hpp"

class dbClient : public dbClientIf
{
public:
	dbClient() = default;
	~dbClient() = default;
	HttpStatus::Code upload_iDomData(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &iDomData, uint64_t timestamp, DATABASE* config);
	HttpStatus::Code uploadBulbData(const std::string &name, bool state, std::optional<uint64_t> timestamp = std::nullopt, DATABASE* config = std::nullptr_t());
	HttpStatus::Code upload_systemData(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &, uint64_t, DATABASE* config);
	HttpStatus::Code upload_universal(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &, uint64_t, DATABASE* config, std::optional<std::string_view> bucket = std::nullopt);
};