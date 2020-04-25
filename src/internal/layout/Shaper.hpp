#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <functional>

#include <unicode/unistr.h>

#ifdef __EMSCRIPTEN__
    #include <hb.h>
#else
    #include <harfbuzz/hb.h>
#endif


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
    static constexpr double FONT_ALTERNATIVE_THRESHOLD = 0.2;

public:
    explicit Shaper(std::shared_ptr<FontTable> fontTable);

    void setTextBuffer(std::shared_ptr<const icu::UnicodeString> text);

    std::vector<ShapeResult> shapeRuns(const std::vector<InternalTextRun> & runs);

private:
    void shapeRun(const InternalTextRun & run,
        std::vector<ShapeResult> & results);

    bool shapeRunWithFont(const InternalTextRun & run, FontID fontID,
        std::vector<ShapeResult> & results, double notDefThreshold);

    void pushBufferResults(const InternalTextRun & run,
        hb_glyph_info_t * glyphInfos,
        hb_glyph_position_t * glyphPositions,
        unsigned int glyphCount, FontID fontID,
        std::vector<ShapeResult> & results);

};

}
