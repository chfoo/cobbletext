#pragma once

#include <unicode/errorcode.h>

namespace cobbletext::internal {

class ICUError : public icu::ErrorCode {
    void handleFailure() const;
};

}
