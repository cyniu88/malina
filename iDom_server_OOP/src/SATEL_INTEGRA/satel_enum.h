#ifndef SATEL_ENUM_H
#define SATEL_ENUM_H


enum INTEGRA_ENUM: unsigned char {
    HEADER_MSG = 0xFE,
    END = 0x0D,
    OUTPUTS_STATE = 0x17,
    VERSION = 0x7E,
    ARMED_PARTITIONS = 0x0A

};

#endif // SATEL_ENUM_H
