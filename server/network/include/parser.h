#pragma once

#include "server.h"
#include "engine.h"
#define HEADER_V1_SIZE 8
#define SET_TTL_FLAG 0x01

/*
PROTOCOL HEADER
===============

Byte:      1        2       3 .. 4    5..8
indicates: version  opcode  FLAGS     PAYLOAD_LEN

PAYLOAD
=======
opcode SET
----------
2_bytes  KEYLEN_bytes  4_bytes       VALUELEN_bytes   4_bytes
KEYLEN   KEY           VALUELEN      VALUE            TTL(if present in flag)

opcode  GET
-----------
2_bytes  KEYLEN_bytes
KEYLEN   KEY

opcode SET_TTL
--------------
2_bytes  KEYLEN_bytes  4_bytes
KEYLEN   KEY           TTL

opcode GET_TTL
--------------
2_bytes  KEYLEN_bytes
KEYLEN   KEY

opcode STAT
-----------
4_bytes
STAT_FLAG (flags mentioning stats required)

opcode CONFIG
-------------
Will be decided in v2 if need remote config
*/

/*

RESPONSE HEADER
===============
Byte:      1        2..3    4..5      6..9
indicates: version  status  FLAGS     PAYLOAD_LEN

PAYLOAD
======
PAYLOAD_LEN bytes

*/

class Parser {
    public:
    static void returnResponse(int status_code, void* data, int data_len, Connection& conn);
    static Command parse(Connection& conn);
    static Command parseV1(Connection& conn);
};