#pragma once

#include "Library.hpp"

#include <memory>
#include <regex>

#include "EngineImpl.hpp"
#include "internal/Context.hpp"

namespace cobbletext {

class Library::Impl {
    const std::regex fragmentPattern = std::regex("(.+)#([^/\\]+)",
        std::regex::extended);

public:

    std::shared_ptr<internal::Context> context;

    Impl();

    FontID loadFont(const std::string & path);

};

}
