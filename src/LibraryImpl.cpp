#include "LibraryImpl.hpp"

#include <filesystem>

namespace cobbletext {

Library::Impl::Impl() :
    context(std::make_shared<internal::Context>())
    {}


FontID Library::Impl::loadFont(const std::string & path) {
    std::filesystem::path fsPath(path);

    auto filenamePart = fsPath.filename().string();
    size_t fragmentIndex = filenamePart.rfind("#");

    if (fragmentIndex != std::string::npos) {
        auto plainFilename = filenamePart.substr(0, fragmentIndex);
        auto fragment = filenamePart.substr(fragmentIndex + 1, std::string::npos);
        size_t numProcessed = 0;
        int32_t faceIndex = 0;

        try {
            faceIndex = std::stoi(fragment, &numProcessed);
        } catch (std::exception & error) {
        }

        if (numProcessed == fragment.size()) {
            auto processedPath = (fsPath.parent_path() / plainFilename).string();
            return context->fontTable->load(processedPath.c_str(), faceIndex);
        }
    }

    return context->fontTable->load(path.c_str());
}

}
