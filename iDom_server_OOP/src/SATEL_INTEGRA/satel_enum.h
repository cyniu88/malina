#ifndef SATEL_ENUM_H
#define SATEL_ENUM_H


enum  INTEGRA_ENUM: unsigned char {
    HEADER_MSG = 0xFE,
    END = 0x0D,
    OUTPUTS_STATE = 0x17,
    VERSION = 0x7E,
    ARMED_PARTITIONS = 0x0A,
    ARM = 0x80,
    DISARM = 0x84,
    OK= 0x00,
    NOT_ARMED = 0x12,
    NOT_ARMED_USE_FORCE = 0x11
};

#endif // SATEL_ENUM_H
