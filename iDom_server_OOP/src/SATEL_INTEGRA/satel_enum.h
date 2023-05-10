#ifndef SATEL_ENUM_H
#define SATEL_ENUM_H

enum INTEGRA_ENUM : unsigned char
{
    HEADER_MSG = 0xFE,
    END = 0x0D,
    OUTPUTS_STATE = 0x17,
    VERSION = 0x7E,
    ARMED_PARTITIONS = 0x0A,
    ARM = 0x80,
    DISARM = 0x84,
    OUTPUT_ON = 0x88,
    OUTPUT_OFF = 0x89,
    OK = 0x00,
    NOT_ARMED = 0x12,
    NOT_ARMED_USE_FORCE = 0x11
};

inline std::ostream &operator<<(std::ostream &os, const INTEGRA_ENUM &v)
{
    switch (v)
    {
    case INTEGRA_ENUM::HEADER_MSG:
        os << "HEADER_MSG";

    case INTEGRA_ENUM::END:
        os << "END";
        break;
    case INTEGRA_ENUM::OUTPUTS_STATE:
        os << "OUTPUTS_STATE";
        break;
    case INTEGRA_ENUM::VERSION:
        os << "VERSION";
        break;
    case INTEGRA_ENUM::ARMED_PARTITIONS:
        os << "ARMED_PARTITIONS";
        break;
    case INTEGRA_ENUM::ARM:
        os << "ARM";
        break;
    case INTEGRA_ENUM::DISARM:
        os << "DISARM";
        break;
    case INTEGRA_ENUM::OUTPUT_ON:
        os << "OUTPUT_ON";
        break;
    case INTEGRA_ENUM::OUTPUT_OFF:
        os << "OUTPUT_OFF";
        break;
    case INTEGRA_ENUM::OK:
        os << "OK";
        break;
    case INTEGRA_ENUM::NOT_ARMED:
        os << "NOT_ARMED";
        break;
    case INTEGRA_ENUM::NOT_ARMED_USE_FORCE:
        os << "NOT_ARMED_USE_FORCE";
        break;
    default:
        os << "Unknown";
    }
    return os;
};

#endif // SATEL_ENUM_H
