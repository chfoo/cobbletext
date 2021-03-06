#pragma once

#include <stdexcept>
#include <string>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {

class COBBLETEXT_CPP_API RuntimeError : public std::runtime_error {
public:
    explicit RuntimeError(const std::string & what_arg);
    explicit RuntimeError(const char* what_arg);
};

class COBBLETEXT_CPP_API LogicError : public std::logic_error {
public:
    explicit LogicError(const std::string & what_arg);
    explicit LogicError(const char* what_arg);
};

class COBBLETEXT_CPP_API LibraryError : public RuntimeError {
public:
    long long int code;
    std::string message;

    explicit LibraryError(std::string message);
    explicit LibraryError(std::string message, long long int code);
};

}
