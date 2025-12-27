#include "../include/api.h"
#include <vector>

void ApiService::setPayloadLength(const std::string &key, const std::string &value, std::vector<uint8_t> &header, const bool include_ttl) {
    int payloadLength = 2 + key.size() + 4 + value.size();
    if (include_ttl) payloadLength += 4;
    int index = 7;
    while (payloadLength > 0) {
        const uint8_t headerValue = (payloadLength | 255);
        header[index--] = headerValue;
        payloadLength = payloadLength >> 8;
    }
}

void ApiService::setKeyLengthPayload(const std::string &key, std::vector<uint8_t> &header) {
    fillKeyLen(header);
    int index = header.size() - 1;
    int val = key.size();

    while (val > 0) {
        const uint8_t headerValue = (val | 255);
        header[index--] = headerValue;
        val = (val >> 8);
    }
}

void ApiService::setValueLengthPayload(const std::string &value, std::vector<uint8_t> &header) {
    fillValueLen(header);
    int index = header.size() - 1;
    int val = value.size();

    while (val > 0) {
        const uint8_t headerValue = (val | 255);
        header[index--] = headerValue;
        val = (val >> 8);
    }
}

void ApiService::setTtlPayload(uint32_t ttl, std::vector<uint8_t> &header) {
    fillTtlLen(header);
    int index = header.size() - 1;

    while (ttl > 0) {
        const uint8_t headerValue = (ttl | 255);
        header[index--] = headerValue;
        ttl = (ttl >> 8);
    }
}



void ApiService::fillKeyLen(std::vector<uint8_t> &header) {
    for (int i = 0; i < keyLen; i++) {
        header.push_back(0);
    }
}

void ApiService::fillValueLen(std::vector<uint8_t> &header) {
    for (int i = 0; i < valueLen; i++) {
        header.push_back(0);
    }
}

void ApiService::fillTtlLen(std::vector<uint8_t> &header) {
    for (int i = 0; i < ttlLen; i++) {
        header.push_back(0);
    }
}

void ApiService::addStringToHeader(const std::string &s, std::vector<uint8_t> &header) {
    for (const uint8_t c : s) {
        header.push_back(c);
    }
}


std::vector<uint8_t> ApiService::set(const std::string key, const std::string value) {
    std::vector<uint8_t> header (8, 0);
    // Version
    header[0] = Version::V1;
    // Operation Code
    header[1] = OperationCode::SET;
    header[2] = 0;
    // Flag
    header[3] = 0;
    header[4]= 0;
    // Payload length
    setPayloadLength(key, value, header, false);

    // Payload
    // Adding the key len
    setKeyLengthPayload(key, header);
    // Adding the key
    addStringToHeader(key, header);
    // Adding the value len
    setValueLengthPayload(value, header);
    // Adding the value
    addStringToHeader(value, header);
    // Note: Since there is no TTL, so we are not adding the TTL.

    return header;
}

std::vector<uint8_t> ApiService::set(std::string key, std::string value, uint32_t ttl) {
    std::vector<uint8_t> header;
    // Version
    header[0] = Version::V1;
    // Operation Code
    header[1] = OperationCode::SET;
    header[2] = 0;
    // Flag
    header[3] = 0;
    header[4]= 1;
    // Payload length
    setPayloadLength(key, value, header, true);

    // Payload
    // Adding the key len
    setKeyLengthPayload(key, header);
    // Adding the key
    addStringToHeader(key, header);
    // Adding the value len
    setValueLengthPayload(value, header);
    // Adding the value
    addStringToHeader(value, header);
    // Adding the TTL value to the header
    setTtlPayload(ttl, header);

    return header;
}

std::vector<uint8_t> ApiService::get(std::string key) {

}

std::vector<uint8_t> ApiService::get_ttl(std::string) {

}

std::vector<uint8_t> ApiService::set_ttl(std::string key, uint32_t ttl) {

}

