#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <memory>

#include <gsl/span>
#include <unicode/ucnv.h>

namespace cobbletext::internal {

class UConverterDeleter {
public:
    void operator()(UConverter * converter) const {
        ucnv_close(converter);
    }
};

class Codec {
    std::unique_ptr<UConverter,UConverterDeleter> utf8Converter;
    std::unique_ptr<UConverter,UConverterDeleter> utf16LEConverter;

public:
    Codec();

    std::u16string_view utf8BytesToUTF16CodeUnits(
        const gsl::span<const uint8_t> & bytes);
    std::u16string_view utf16LEBytesToUTF16CodeUnits(
        const gsl::span<const uint8_t> & bytes);

private:
    std::u16string_view decodeWithConverter(UConverter * converter,
        const gsl::span<const uint8_t> & bytes);
};

}
