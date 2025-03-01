#pragma once

#include <json.hpp>


class RecuperatorInterface {
public:
    // Destructor
    virtual ~RecuperatorInterface() = default;

    // Pure virtual functions to be implemented by derived classes
    virtual std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> getData() = 0;
    virtual void setDataFromMqtt(const std::pair<std::string, std::string>& data) = 0;
    virtual void stop() = 0;
    virtual void setSpeed(const std::string_view speed) = 0;

};

