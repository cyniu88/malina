#pragma once

#include <gmock/gmock.h>

#include "../db-client-if.hpp"

class dbClient_MOCK : public dbClientIf
{
public:
    ~dbClient_MOCK() = default;
    MOCK_METHOD1(upload_iDomData, HttpStatus::Code(const std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &, DATABASE* config));
	MOCK_METHOD2(uploadBulbData, HttpStatus::Code(const std::string &name, bool state, DATABASE* config));
    MOCK_METHOD1(upload_systemData, HttpStatus::Code(const std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &, DATABASE* config));
    MOCK_METHOD1(upload_universal, HttpStatus::Code(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &, uint64_t, DATABASE *config, std::optional<std::string_view> bucket = std::nullopt));
};