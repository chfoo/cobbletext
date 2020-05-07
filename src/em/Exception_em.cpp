#include "Exception.hpp"

#include <emscripten/bind.h>

using namespace emscripten;


namespace cobbletext::em {

std::string getExceptionMessage(intptr_t pointer) {
    auto error = reinterpret_cast<std::exception*>(pointer);
    return std::string(error->what());
}

}

EMSCRIPTEN_BINDINGS(cobbletext_Exception) {
    class_<cobbletext::RuntimeError>("RuntimeError")
        // .function("what", &cobbletext::RuntimeError::what)
        ;

    class_<cobbletext::LogicError>("LogicError")
        // .function("what", &cobbletext::LogicError::what)
        ;

    class_<cobbletext::LibraryError>("LibraryError")
        .property("code", &cobbletext::LibraryError::code)
        .property("message", &cobbletext::LibraryError::message)
        // .function("what", &cobbletext::LibraryError::what)
        ;

    function("getExceptionMessage", &cobbletext::em::getExceptionMessage,
        allow_raw_pointers());
}
