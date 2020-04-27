#include "LibraryImpl.hpp"

namespace cobbletext {

Library::Impl::Impl() :
    context(std::make_shared<internal::Context>())
    {}


FontID Library::Impl::loadFont(const std::string & path) {
    std::smatch fragmentMatch;

    if (std::regex_match(path, fragmentMatch, fragmentPattern)) {
        auto fragment = fragmentMatch.str(2);
        size_t numProcessed = 0;
        int32_t faceIndex = 0;

        try {
            faceIndex = std::stoi(fragment, &numProcessed);
        } catch (std::exception & error) {
        }

        if (numProcessed == fragment.size()) {
            auto processedPath = fragmentMatch.str(1);
            return context->fontTable->load(processedPath.c_str(), faceIndex);
        }
    }

    return context->fontTable->load(path.c_str());
}

}
