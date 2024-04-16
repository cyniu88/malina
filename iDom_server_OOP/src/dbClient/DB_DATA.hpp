#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <any>

class DB_DATA
{
public:
   uint64_t timestamp;
   std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
   DB_DATA(uint64_t _timestamp, const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>>& _data):
   timestamp(_timestamp), data(_data)
   {
    
   }
};