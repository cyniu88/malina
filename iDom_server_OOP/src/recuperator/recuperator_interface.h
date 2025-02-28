#pragma once
#include <string_view>

class RecuperatorInterface {
public:
    // Constructor
    RecuperatorInterface() {}

    // Destructor
    virtual ~RecuperatorInterface() {}

    // Pure virtual functions to be implemented by derived classes
    virtual void getData() = 0;
    virtual void stop() = 0;
    virtual void setSpeed(const std::string_view speed) = 0;
};

