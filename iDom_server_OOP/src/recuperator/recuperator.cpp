#include "recuperator.h"
#include <iostream>

Recuperator::Recuperator(thread_context *d_context):context(d_context) {
    // Constructor implementation
}

Recuperator::~Recuperator() {
    // Destructor implementation
}

std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> Recuperator::getData() {
    std::lock_guard<std::mutex> lock(mtx);
    return recuData;
}

void Recuperator::setDataFromMqtt(const std::pair<std::string, std::string>& data)
{
    // temporaty event to phase 1
    auto ss = data.first + " " + data.second;
    context->myEventHandler.run("recuperation")->addEvent(ss);
    auto pos = data.first.find_last_of('/');
    if (pos != std::string::npos) {
        std::string extracted = data.first.substr(pos + 1);
        std::cout << "Extracted string: " << extracted << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        recuData["recuperator"][data.first.substr(pos + 1)] = std::stof(data.second);
    }
}

void Recuperator::stop() {
    // Method to stop the recuperator
    std::cout << "Stopping recuperator" << std::endl;
}

void Recuperator::setSpeed(const std::string_view speed) {
    // Method to set the speed of the recuperator
    std::cout << "Setting recuperator speed to " << speed << std::endl;
}