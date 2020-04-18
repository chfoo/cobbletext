#include "LibraryImpl.hpp"

namespace cobbletext {

Library::Impl::Impl() :
    context(std::make_shared<internal::Context>())
    {}

}
