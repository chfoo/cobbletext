#pragma once

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

namespace cobbletext::internal {

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

template <class E>
[[noreturn]] void throw_with_trace(const E& e) {
    throw boost::enable_error_info(e)
        << traced(boost::stacktrace::stacktrace());
}

using boost::get_error_info;

}
