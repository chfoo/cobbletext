#include "Exception.hpp"

namespace cobbletext {

RuntimeError::RuntimeError(const std::string & what_arg) :
    std::runtime_error(what_arg) {}

RuntimeError::RuntimeError(const char * what_arg) :
    std::runtime_error(what_arg) {}

LogicError::LogicError(const std::string & what_arg) :
    std::logic_error(what_arg) {}

LogicError::LogicError(const char * what_arg) :
    std::logic_error(what_arg) {}

LibraryError::LibraryError(std::string message) :
    RuntimeError(message),
    code(-1),
    message(message) {}

LibraryError::LibraryError(std::string message, long long int code) :
    RuntimeError(std::to_string(code) + " : " + message),
    code(code),
    message(message) {}

}
