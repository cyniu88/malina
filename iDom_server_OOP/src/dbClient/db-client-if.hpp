#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <utility>

#include "../../libs/httpStatusCode/httpStatusCode.hpp"

class dbClientIf
{
public:
	virtual HttpStatus::Code upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &) = 0;
	virtual HttpStatus::Code uploadBulbData(const std::string &, bool) = 0;

	virtual ~dbClientIf() = default;
};