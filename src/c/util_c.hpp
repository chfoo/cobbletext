#pragma once

#include <exception>

#include "cobbletext.h"

namespace cobbletext::c {

void handleSuccess(struct CobbletextLibrary * library);

void handleException(struct CobbletextLibrary * library,
    std::exception * exception);

}
