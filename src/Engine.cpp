#include "Engine.hpp"
#include "EngineImpl.hpp"

#include <gsl/span>
#include <unicode/ustring.h>
#include <unicode/stringpiece.h>

#include "LibraryImpl.hpp"

namespace cobbletext {

Engine::Engine(std::shared_ptr<Library> library) :
    impl(std::make_unique<Impl>(*this, library->impl->context)) {}

std::vector<TileInfo> Engine::tiles() {
    return impl->layoutEngine->tiles();
}

std::vector<AdvanceInfo> Engine::advances() {
    return impl->layoutEngine->advances();
}

void Engine::addText(const uint8_t * data, uint32_t length, Encoding encoding) {
    switch (encoding) {
        case Encoding::UTF8: {
            auto u16text = impl->codec.utf8BytesToUTF16CodeUnits(
                gsl::span(data, length)
            );
            addTextUTF16(u16text);
            delete [] u16text.data();
            break;
        }
        case Encoding::UTF16LE: {
            auto u16text = impl->codec.utf16LEBytesToUTF16CodeUnits(
                gsl::span(data, length)
            );
            addTextUTF16(u16text);
            delete [] u16text.data();
            break;
        }
    }
}

void Engine::addTextUTF8(std::string_view text) {
    auto stringPiece = icu::StringPiece(text.data(), text.size());
    auto uString = icu::UnicodeString::fromUTF8(stringPiece);
    impl->addUnicodeString(uString);
}

void Engine::addTextUTF8(const char * text, int32_t length) {
    icu::StringPiece stringPiece;

    if (length >= 0) {
        stringPiece = icu::StringPiece(text, length);
    } else {
        stringPiece = icu::StringPiece(text);
    }

    auto uString = icu::UnicodeString::fromUTF8(stringPiece);
    impl->addUnicodeString(uString);
}

void Engine::addTextUTF16(std::u16string_view text) {
    impl->addUnicodeString(icu::UnicodeString(text.data(), text.size()));
}

void Engine::addTextUTF16(const char16_t * text, int32_t length) {
    if (length >= 0) {
        impl->addUnicodeString(icu::UnicodeString(text, length));
    } else {
        impl->addUnicodeString(icu::UnicodeString(text));
    }

}

void Engine::addTextUTF32(std::u32string_view text) {
    auto uString = icu::UnicodeString::fromUTF32(
        reinterpret_cast<const UChar32*>(text.data()), text.size());
    impl->addUnicodeString(uString);
}

void Engine::addTextUTF32(const char32_t * text, int32_t length) {
    icu::UnicodeString uString = icu::UnicodeString::fromUTF32(
        reinterpret_cast<const UChar32*>(text), length);

    impl->addUnicodeString(uString);
}

void Engine::addInlineObject(InlineObjectID id, uint32_t size) {
    impl->addInlineObject(id, size);
}

void Engine::clear() {
    impl->clear();
}

void Engine::layOut() {
    impl->layOut();
}



bool Engine::tilesValid() {
    return impl->tilesValid();
}

void Engine::rasterize() {
    impl->rasterize();
}

bool Engine::packTiles(uint32_t width, uint32_t height) {
    return impl->packTiles(width, height);
}

}
