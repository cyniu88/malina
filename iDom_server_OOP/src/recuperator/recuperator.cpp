#include "recuperator.h"
#include <iostream>
#include <json.hpp>

Recuperator::Recuperator(thread_context *d_context) : context(d_context)
{
    // Constructor implementation
}

Recuperator::~Recuperator()
{
    // Destructor implementation
}

std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> Recuperator::getData()
{
    std::lock_guard<std::mutex> lock(mtx);
    recuData.erase("recuperator");

    std::cout << "recuData size: " << recuData.size() << std::endl;

    for (auto &[name, data] : recuDB)
    {
        std::cout << " dupaaa : " << name << "  wartosc: " << data.average() << std::endl;
        recuData["recuperator"][name] = data.average();
        data.erase();
    }
    return recuData;
}

void Recuperator::setDataFromMqtt(const std::pair<std::string, std::string> &data)
{
    // temporaty event to phase 1
    auto ss = data.first + " " + data.second;
    context->myEventHandler.run("recuperation")->addEvent(ss);
    auto pos = data.first.find_last_of('/');
    if (pos != std::string::npos)
    {
        std::string extracted = data.first.substr(pos + 1);
#ifdef BT_TEST
        std::cout << "Extracted string: " << extracted << std::endl;
#endif
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        // recuData["recuperator"][data.first.substr(pos + 1)] = std::stof(data.second);
        recuDB[data.first.substr(pos + 1)].push_back(std::stof(data.second));
    }
}

void Recuperator::stop()
{
    // Method to stop the recuperator
    std::cout << "Stopping recuperator" << std::endl;
}

void Recuperator::setSpeed(const std::string_view speed)
{

    auto logError = [this, &speed]()
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "błąd ustawiania predkości rekuperacji na " << speed << std::endl;
        log_file_mutex.mutex_unlock();
    };

    auto sendTopic = context->server_settings->_recuperation.MQTT_CONTROL_TOPIC;

    if (speed == "away" or speed == "low" or speed == "medium" or speed == "high")
    {
        auto ret = context->mqttHandler->publish(sendTopic, std::string(speed));
        if (ret == 0)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << INFO << "ustawiono prędkość rekuperacji na: " << speed << std::endl;
            log_file_mutex.mutex_unlock();
        }
        else
        {
            logError();
        }
        return;
    }

    logError();
}

void Recuperator::setAwayModeTime(bool state, int time, std::string_view unit)
{
    if (time > 0)
    {
        nlohmann::json j;
        j["state"] = state;
        j["time"] = time;
        j["unit"] = unit;
        context->mqttHandler->publish(context->server_settings->_recuperation.MQTT_CONTROL_TOPIC + "/away/set", j.dump());
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "ustawiono tryb nieobecności w  rekuperacji na: " << time << " " << unit << std::endl;
    log_file_mutex.mutex_unlock();
}

void Recuperator::setAwayMode()
{
    context->mqttHandler->publish(context->server_settings->_recuperation.MQTT_CONTROL_TOPIC + "/away/set", "true");
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "ustawiono tryb nieobecności w  rekuperacji na stałe " << std::endl;
    log_file_mutex.mutex_unlock();
}

void Recuperator::setBoostMode(int time, std::string_view unit, bool state)
{
    if (time > 0)
    {
        nlohmann::json j;
        j["state"] = state;
        j["time"] = time;
        j["unit"] = unit;
        context->mqttHandler->publish(context->server_settings->_recuperation.MQTT_CONTROL_TOPIC + "/boost/set", j.dump());
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "Problem z ustawieniem przewietrzania" << time << " " << unit << std::endl;
        log_file_mutex.mutex_unlock();
    }
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "ustawiono tryb przewietrzania w  rekuperacji na: " << time << " " << unit << std::endl;
    log_file_mutex.mutex_unlock();
}
