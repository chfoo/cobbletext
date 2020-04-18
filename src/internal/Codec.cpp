#include "internal/Codec.hpp"

#include <unicode/ucnv.h>

#include "internal/ICUError.hpp"

namespace cobbletext::internal {

Codec::Codec() {
    ICUError errorCode;

    utf8Converter.reset(ucnv_open("UTF-8", errorCode));
    utf16LEConverter.reset(ucnv_open("UTF-16LE", errorCode));
}

std::u16string_view Codec::utf8BytesToUTF16CodeUnits(
        const gsl::span<const uint8_t> & bytes) {
    return decodeWithConverter(utf8Converter.get(), bytes);
}

std::u16string_view Codec::utf16LEBytesToUTF16CodeUnits(
        const gsl::span<const uint8_t> & bytes) {
    return decodeWithConverter(utf16LEConverter.get(), bytes);
}

std::u16string_view Codec::decodeWithConverter(UConverter * converter,
        const gsl::span<const uint8_t> & bytes) {

    ICUError errorCode;
    auto bufferSize = bytes.size() * 2;
    auto buffer = new char16_t[bufferSize];
    auto size = ucnv_toUChars(converter, buffer, bufferSize,
        (const char *) bytes.data(), bytes.size(), errorCode);

    return std::u16string_view(buffer, size);
}

}
