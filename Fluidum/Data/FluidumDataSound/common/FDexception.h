#pragma once

#include <exception>
#include <string>

namespace FD::Sound {

    //重大な内部エラー
    class InternalError : public std::exception {
    public:
        using Base = exception;

        explicit InternalError(const std::string& message) : Base(message.c_str()) {}

        explicit InternalError(const char* message) : Base(message) {}

    };

}

namespace FD::Piano::Internal {

    inline constexpr const char* const ExceptionMessageOutOfRangeKey = "Key out of range.";

}