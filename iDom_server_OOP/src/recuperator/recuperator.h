#pragma once

#include "recuperator_interface.h"
#include "../iDom_server_OOP.h"

struct thread_context;

class Recuperator : public RecuperatorInterface
{
public:
    explicit Recuperator(thread_context *d_context);
    ~Recuperator();

    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> getData();
    void setDataFromMqtt(const std::pair<std::string, std::string>& data);
    void stop();
    void setSpeed(const std::string_view speed);

private:
    std::mutex mtx;
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> recuData;
    //iDomData["temperatura"]["outdoor"] = -22.55;
    thread_context *context;
};
