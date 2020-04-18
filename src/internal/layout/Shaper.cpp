#include "internal/layout/Shaper.hpp"

namespace cobbletext::internal {

Shaper::Shaper(std::shared_ptr<FontTable> fontTable) :
        fontTable(fontTable),
        harfBuzzBuffer(hb_buffer_create()) {
    hb_buffer_reference(harfBuzzBuffer.get());
}

void Shaper::setText(const icu::UnicodeString & text) {
    this->text = std::ref(text);
}

std::vector<ShapeResult> Shaper::shapeRuns(const std::vector<InternalTextRun> & runs) {
    std::vector<ShapeResult> results;

    for ( auto const & run : runs) {
        shapeRun(run, results);
    }

    return results;
}

void Shaper::shapeRun(const InternalTextRun & run, std::vector<ShapeResult> & results) {
    auto font = fontTable->getFont(run.source.textFormat->fontFace);

    hb_buffer_clear_contents(harfBuzzBuffer.get());

    hb_buffer_set_direction(harfBuzzBuffer.get(), run.direction);
    hb_buffer_set_script(harfBuzzBuffer.get(), run.script);
    hb_buffer_set_language(harfBuzzBuffer.get(), run.language);

    hb_buffer_add_utf16(harfBuzzBuffer.get(),
        reinterpret_cast<const uint16_t*>(text->get().getBuffer()),
        text->get().length(),
        run.textIndex, run.textLength
    );

    FT_Set_Char_Size(font.freeTypeFace,
        0, run.source.textFormat->fontSize * 64,
        0, 0);

    hb_shape(font.harfBuzzFont, harfBuzzBuffer.get(), nullptr, 0);

    unsigned int glyphCount;

    hb_glyph_info_t * glyphInfos = hb_buffer_get_glyph_infos(
        harfBuzzBuffer.get(), &glyphCount);
    hb_glyph_position_t * glyphPositions = hb_buffer_get_glyph_positions(
        harfBuzzBuffer.get(), &glyphCount);

    for (unsigned int index = 0; index < glyphCount; index++) {
        auto glyphInfo = glyphInfos[index];
        auto glyphPosition = glyphPositions[index];

        ShapeResult result = ShapeResult(run);
        result.xAdvance = glyphPosition.x_advance / 64.0;
        result.yAdvance = glyphPosition.y_advance / 64.0;
        result.xOffset = glyphPosition.x_offset / 64.0;
        result.yOffset = glyphPosition.y_offset / 64.0;
        result.glyphIndex = glyphInfo.codepoint;
        result.cluster = glyphInfo.cluster;

        results.push_back(result);
    }
}

}