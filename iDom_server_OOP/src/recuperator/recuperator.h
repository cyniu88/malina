#pragma once
#include <string_view>

#include "recuperator_interface.h"

class Recuperator : public RecuperatorInterface {
public:
    Recuperator();
    ~Recuperator();

    void getData();
    void stop();
    void setSpeed(const std::string_view speed);
   
};


