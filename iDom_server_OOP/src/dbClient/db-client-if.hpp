#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <utility>

class dbClientIf
{
public:
	virtual int upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &) = 0;
	virtual int uploadBulbData(const std::string &, bool) = 0;

	virtual ~dbClientIf() = default;
};