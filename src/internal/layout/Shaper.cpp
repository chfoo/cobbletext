#include "internal/layout/Shaper.hpp"

#include <cassert>

namespace cobbletext::internal {

Shaper::Shaper(std::shared_ptr<FontTable> fontTable) :
        fontTable(fontTable),
        harfBuzzBuffer(hb_buffer_create()) {

    hb_buffer_reference(harfBuzzBuffer.get());
    hb_buffer_set_cluster_level(harfBuzzBuffer.get(),
        HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
}

void Shaper::setTextBuffer(std::shared_ptr<const icu::UnicodeString> text) {
    this->text = text;
}

std::vector<ShapeResult> Shaper::shapeRuns(
        const std::vector<InternalTextRun> & runs) {
    std::vector<ShapeResult> results;

    for ( auto const & run : runs) {
        shapeRun(run, results);
    }

    return results;
}

void Shaper::shapeRun(const InternalTextRun & run,
        std::vector<ShapeResult> & results) {
    FontID fontID = run.source.textFormat.fontFace;

    if (!fontTable->hasFont(fontID)) {
        fontID = fontTable->fallbackFont;
    }

    auto font = fontTable->getFont(fontID);

    hb_buffer_clear_contents(harfBuzzBuffer.get());

    hb_buffer_set_direction(harfBuzzBuffer.get(), run.direction);
    hb_buffer_set_script(harfBuzzBuffer.get(), run.script);
    hb_buffer_set_language(harfBuzzBuffer.get(), run.language);

    hb_buffer_add_utf16(harfBuzzBuffer.get(),
        reinterpret_cast<const uint16_t*>(text->getBuffer()),
        text->length(),
        run.textIndex, run.textLength
    );

    auto hasChanged = fontTable->setFontSize(fontID,
        run.source.textFormat.fontSize);

    if (hasChanged) {
        hb_ft_font_changed(font.harfBuzzFont);
    }

    hb_shape(font.harfBuzzFont, harfBuzzBuffer.get(), nullptr, 0);

    unsigned int glyphCount;

    hb_glyph_info_t * glyphInfos = hb_buffer_get_glyph_infos(
        harfBuzzBuffer.get(), &glyphCount);
    hb_glyph_position_t * glyphPositions = hb_buffer_get_glyph_positions(
        harfBuzzBuffer.get(), &glyphCount);

    if (!glyphCount) {
        return;
    }

    for (size_t index_ = 0; index_ < glyphCount; index_++) {
        size_t index;

        if (run.direction == HB_DIRECTION_RTL) {
            index = glyphCount - index_ - 1;
        } else {
            index = index_;
        }

        auto glyphInfo = glyphInfos[index];
        auto glyphPosition = glyphPositions[index];

        ShapeResult result = ShapeResult(run);
        result.xAdvance = glyphPosition.x_advance / 64.0;
        result.yAdvance = glyphPosition.y_advance / 64.0;
        result.xOffset = glyphPosition.x_offset / 64.0;
        result.yOffset = glyphPosition.y_offset / 64.0;
        result.glyphIndex = glyphInfo.codepoint;
        result.cluster = glyphInfo.cluster;

        #ifdef COBBLETEXT_DEBUG
        result.codePoint = text->char32At(glyphInfo.cluster);
        #endif

        results.push_back(result);
    }
}

}
