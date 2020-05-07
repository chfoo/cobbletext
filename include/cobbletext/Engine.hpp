#pragma once

#include <string>
#include <memory>

#include "AdvanceInfo.hpp"
#include "common.hpp"
#include "macros.h"
#include "Encoding.hpp"
#include "FontInfo.hpp"
#include "GlyphInfo.hpp"
#include "Library.hpp"
#include "OutputInfo.hpp"
#include "ScriptDirection.hpp"
#include "TileInfo.hpp"
#include "TextAlignment.hpp"

namespace cobbletext {


class COBBLETEXT_CPP_API Engine {
public:
    uint32_t lineLength = 0;
    std::string locale;
    TextAlignment textAlignment = TextAlignment::NotSpecified;
    std::string language;
    std::string script;
    ScriptDirection scriptDirection = ScriptDirection::NotSpecified;
    FontID font = 0;
    double fontSize = 12;
    CustomPropertyID customProperty = 0;
    OutputInfo outputInfo;

    std::vector<TileInfo> tiles();
    std::vector<AdvanceInfo> advances();

    explicit Engine(std::shared_ptr<Library> library);
    ~Engine();

    void addText(const uint8_t * data, uint32_t length, Encoding encoding);
    void addTextUTF8(std::string_view text);
    void addTextUTF8(const char * text, int32_t length);
    void addTextUTF16(std::u16string_view text);
    void addTextUTF16(const char16_t * text, int32_t length);
    void addTextUTF32(std::u32string_view text);
    void addTextUTF32(const char32_t * text, int32_t length);

    void addInlineObject(InlineObjectID id, uint32_t width, uint32_t height);

    void clear();
    void layOut();

    bool tilesValid();

    void rasterize();

    bool packTiles(uint32_t width, uint32_t height);
    void clearTiles();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

}
