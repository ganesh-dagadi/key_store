#pragma once
#include <string>
#include <vector>

enum OperationCode : uint8_t {
    SET,
    GET,
    SET_TTL,
    GET_TTL
};

enum Version : uint8_t {
    V1, V2
};

class ApiService {
private:
    static constexpr int keyLen = 2;
    static constexpr int valueLen = 4;
    static constexpr int ttlLen = 4;
    static void setPayloadLength(const std::string &key, const std::string &value, std::vector<uint8_t> &header, const bool include_ttl);
    static void setKeyLengthPayload(const std::string &key, std::vector<uint8_t> &header);
    static void setValueLengthPayload(const std::string &value, std::vector<uint8_t> &header);
    static void setTtlPayload(uint32_t ttl, std::vector<uint8_t> &header);
    static void fillKeyLen(std::vector<uint8_t> &header);
    static void fillValueLen(std::vector<uint8_t> &header);
    static void fillTtlLen(std::vector<uint8_t> &header);
    static void addStringToHeader(const std::string &s, std::vector<uint8_t> &header);
public:
    static std::vector<uint8_t> set(std::string key, std::string value, uint32_t ttl);
    static std::vector<uint8_t> set(const std::string key, const std::string value);
    static std::vector<uint8_t> get(std::string key);
    static std::vector<uint8_t> set_ttl(std::string key, uint32_t ttl);
    static std::vector<uint8_t> get_ttl(std::string);
};
