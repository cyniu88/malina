#pragma once

#include <stdexcept>
#include <string>

/// Thrown when the data format / packet structure is wrong.
class WRONG_FORMAT : public std::exception {};

/// Thrown when a requested item is not found (e.g. radio equipment, argument in message).
class NOT_FOUND_ERROR : public std::runtime_error {
public:
    explicit NOT_FOUND_ERROR(const std::string &msg) : std::runtime_error(msg) {}
};

/// Thrown on serial-port / hardware communication failure.
class SERIAL_PORT_ERROR : public std::runtime_error {
public:
    explicit SERIAL_PORT_ERROR(const std::string &msg) : std::runtime_error(msg) {}
};

/// Thrown when a Satel Integra command receives a bad / empty response.
class SATEL_RECV_ERROR : public std::runtime_error {
public:
    explicit SATEL_RECV_ERROR(const std::string &msg) : std::runtime_error(msg) {}
};

/// Thrown when a thermometer is not found in the container.
class THERMOMETER_NOT_FOUND : public std::runtime_error {
public:
    explicit THERMOMETER_NOT_FOUND(const std::string &msg) : std::runtime_error(msg) {}
};

/// Thrown when the tokenizer receives an empty command string.
class EMPTY_COMMAND_ERROR : public std::runtime_error {
public:
    explicit EMPTY_COMMAND_ERROR(const std::string &msg) : std::runtime_error(msg) {}
};

/// Thrown when connection to MQTT broker fails.
class MQTT_CONNECTION_ERROR : public std::runtime_error {
public:
    explicit MQTT_CONNECTION_ERROR(const std::string &msg) : std::runtime_error(msg) {}
};
