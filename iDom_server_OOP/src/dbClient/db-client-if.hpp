#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <utility>
#include <optional>
#include <stdint.h>

#include "../../libs/httpStatusCode/httpStatusCode.hpp"

class dbClientIf
{
public:
	virtual HttpStatus::Code upload_iDomData(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &, uint64_t ) = 0;
	virtual HttpStatus::Code uploadBulbData(const std::string &, bool, std::optional<uint64_t> timestamp = std::nullopt) = 0;

	virtual ~dbClientIf() = default;
};