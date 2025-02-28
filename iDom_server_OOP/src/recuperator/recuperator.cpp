#include "recuperator.h"
#include <iostream>

Recuperator::Recuperator(thread_context *d_context):context(d_context) {
    // Constructor implementation
}

Recuperator::~Recuperator() {
    // Destructor implementation
}

void Recuperator::getData() {
    // Method to get data
    std::cout << "Getting data from recuperator" << std::endl;
}

void Recuperator::setDataFromMqtt(std::pair<std::string, std::string> data)
{
    // temporaty event to phase 1
    auto ss = data.first + " " + data.second;
    context->myEventHandler.run("recuperation")->addEvent(ss);
}

void Recuperator::stop() {
    // Method to stop the recuperator
    std::cout << "Stopping recuperator" << std::endl;
}

void Recuperator::setSpeed(const std::string_view speed) {
    // Method to set the speed of the recuperator
    std::cout << "Setting recuperator speed to " << speed << std::endl;
}