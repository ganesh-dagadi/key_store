#pragma once

#include <string>
enum class Opcode :uint8_t{
    SET,
    GET,
    SET_TTL,
    GET_TTL,
    STAT
};


class Command {
    public:
    Opcode op_code;
    std::string key;
    std::string value;
    bool set_ttl = false;
    long ttl;
};