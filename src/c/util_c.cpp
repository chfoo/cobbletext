#include "c/util_c.hpp"

#include "library_c.hpp"
#include "Exception.hpp"

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
}

}
