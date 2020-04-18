#pragma once

#include "Library.hpp"

#include <memory>

#include "EngineImpl.hpp"
#include "internal/Context.hpp"

namespace cobbletext {

class Library::Impl {

public:

    std::shared_ptr<internal::Context> context;

    Impl();
};

}
