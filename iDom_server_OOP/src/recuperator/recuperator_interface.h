#pragma once
#include <string_view>
#include <json.hpp>


class RecuperatorInterface {
public:
    // Destructor
    virtual ~RecuperatorInterface() = default;

    // Pure virtual functions to be implemented by derived classes
    virtual void getData() = 0;
    virtual void setDataFromMqtt(std::pair<std::string,std::string>) = 0;
    virtual void stop() = 0;
    virtual void setSpeed(const std::string_view speed) = 0;

};

