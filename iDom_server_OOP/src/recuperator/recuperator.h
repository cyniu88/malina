#pragma once

#include "recuperator_interface.h"
#include "../iDom_server_OOP.h"

struct thread_context;

class Recuperator : public RecuperatorInterface
{
public:
    explicit Recuperator(thread_context *d_context);
    ~Recuperator();

    void getData();
    void setDataFromMqtt(std::pair<std::string, std::string> data);
    void stop();
    void setSpeed(const std::string_view speed);

private:
    std::mutex mtx;
    nlohmann::json dataBases;
    thread_context *context;
};
