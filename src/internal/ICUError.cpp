#include "internal/ICUError.hpp"

#include "Exception.hpp"
#include "internal/ExceptionUtil.hpp"

namespace cobbletext::internal {

void ICUError::handleFailure() const {
    throw_with_trace(LibraryError(u_errorName(errorCode), errorCode));
}

}
