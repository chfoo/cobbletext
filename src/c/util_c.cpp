#include "c/util_c.hpp"

#include "library_c.hpp"
#include "Exception.hpp"
#include "internal/Debug.hpp"
#include "internal/ExceptionUtil.hpp"

namespace cobbletext::c {

void handleSuccess(struct CobbletextLibrary * library) {
    library->errorCode = 0;
    library->errorMessage = "";
}

void handleException(struct CobbletextLibrary * library,
        std::exception * exception) {
    auto * libraryError = dynamic_cast<cobbletext::LibraryError*>(exception);

    if (libraryError) {
        library->errorCode = libraryError->code;
        library->errorMessage = libraryError->message;
    } else {
        library->errorCode = -1;
        library->errorMessage = exception->what();
    }

    #if COBBLETEXT_DEBUG
    auto & exceptionRef = *exception;
    const boost::stacktrace::stacktrace * stackTrace =
        cobbletext::internal::get_error_info<cobbletext::internal::traced>(exceptionRef);

    if (stackTrace) {
        COBBLETEXT_DEBUG_PRINT(*stackTrace)
    } else {
        COBBLETEXT_DEBUG_PRINT("no stack trace available");
    }
    #endif
}

}
