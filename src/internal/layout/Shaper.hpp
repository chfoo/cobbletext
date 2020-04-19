#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <functional>

#include <harfbuzz/hb.h>
#include <unicode/unistr.h>

#include "internal/layout/InternalTextRun.hpp"
#include "internal/font/FontTable.hpp"
#include "internal/layout/ShapeResult.hpp"

namespace cobbletext::internal {

class HarfBuzzBufferDeleter {
public:
    void operator()(hb_buffer_t * buffer) const {
        hb_buffer_destroy(buffer);
    }
};

class Shaper {

private:
    std::shared_ptr<FontTable> fontTable;
    std::unique_ptr<hb_buffer_t,HarfBuzzBufferDeleter> harfBuzzBuffer;
    std::shared_ptr<const icu::UnicodeString> text;

public:
    explicit Shaper(std::shared_ptr<FontTable> fontTable);

    void setTextBuffer(std::shared_ptr<const icu::UnicodeString> text);

    std::vector<ShapeResult> shapeRuns(const std::vector<InternalTextRun> & runs);

private:
    void shapeRun(const InternalTextRun & run,
        std::vector<ShapeResult> & results);

};

}
