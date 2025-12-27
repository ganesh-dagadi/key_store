#include "parser.h"
#include "serverErrors.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <arpa/inet.h> // ntohs, ntohl

void parseV1SetPayload(Connection& conn, Command& cmd, int payload_len) {
    std::vector<uint8_t> payload(payload_len, 0);
    int bytes_read = conn.readExact(payload.data(), payload_len);
    if (bytes_read < payload_len) {
        std::cerr << "Unable to read payload for connection: " << conn.getAddress() << "\n";
        return;
    }
    uint16_t key_len;
    memcpy(&key_len, payload.data(), 2);
    key_len = ntohs(key_len);
    int offset = 2;
    cmd.key = std::string(reinterpret_cast<char*>(payload.data() + offset), key_len);
    offset += key_len;
    uint32_t value_len;
    memcpy(&value_len, payload.data() + offset, 4);
    value_len = ntohl(value_len);
    offset += 4;
    cmd.value = std::string(reinterpret_cast<char*>(payload.data() + offset), value_len);
    offset += value_len;
    if(cmd.set_ttl) {
        uint32_t ttl;
        memcpy(&ttl, payload.data() + offset, 4);
        ttl = ntohl(ttl);
        offset += 4;
        cmd.ttl = static_cast<long>(ttl);
    }
}
Command Parser::parse(Connection& conn) {
    uint8_t version_buf;
    int read_res = conn.readExact(&version_buf, 1);
    if(read_res == 0) {
        // do nothing as already Exception is throws from Connection
    }
    switch(static_cast<HeaderVersion>(version_buf)){
        case HeaderVersion::V1: return Parser::parseV1(conn);
    }
}

Command Parser::parseV1(Connection& conn) {
    uint8_t header_buff[HEADER_V1_SIZE];
    Command cmd;
    // HEADER_V1_SIZE - 1 as version byte is already read
    int bytes_read = conn.readExact(header_buff, HEADER_V1_SIZE - 1);

    if (bytes_read < HEADER_V1_SIZE - 1) throw ServerError("Unable to read Header for connection: "
            + conn.getAddress());
    uint8_t opcode = header_buff[0];
    switch (static_cast<Opcode>(opcode)) {
        case Opcode::SET: cmd.op_code = Opcode::SET;
        break;
        case Opcode::GET: cmd.op_code = Opcode::GET;
        break;
        // handle others
        default: throw ParsingError("Unknown op code");
    }
    uint16_t flags;
    memcpy(&flags, header_buff + 1, 2);
    flags = ntohs(flags);
    if(flags & SET_TTL_FLAG) cmd.set_ttl = true;
    uint32_t payload_len;
    memcpy(&payload_len, header_buff + 3, 4);
    payload_len = ntohl(payload_len);
    std::cout << "payload len" << (int)payload_len << std::endl;
    switch (cmd.op_code) {
    case Opcode::SET:
        parseV1SetPayload(conn, cmd, payload_len);
        break;
    
    default:
        break;
    }
    return cmd;
}

void Parser::returnResponse(int status_code, void* data, int data_len, Connection& conn) {
    int version1HeaderLen = 9;
    int offset = 0;
    std::vector<uint8_t> data_to_send(version1HeaderLen + data_len);
    data_to_send[0] = 0; //version
    offset += 1;
    status_code = htons(status_code);
    memcpy(data_to_send.data() + offset, &status_code, 2);
    offset += 2;
    //currently no use of flags
    uint16_t flags = 0;
    memcpy(data_to_send.data() + offset, &flags, 2);
    offset += 2;
    uint32_t data_len_u = static_cast<uint32_t>(data_len);
    data_len_u = htonl(data_len_u);
    memcpy(data_to_send.data() + offset, &data_len_u, 4);
    offset += 4;
    memcpy(data_to_send.data() + offset, static_cast<char*>(data), data_len);
    offset += data_len;
    conn.writeExact(data_to_send.data(), version1HeaderLen + data_len);
}