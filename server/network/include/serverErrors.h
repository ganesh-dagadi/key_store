#pragma once

#include <exception>
#include <string>


class ParsingError : public std::exception {
private:
    std::string message;

public:
    explicit ParsingError(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ServerError : public std::exception {
private:
    std::string message;

public:
    explicit ServerError(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ConnectionException : public std::exception {
private:
    std::string message;

public:
    explicit ConnectionException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};