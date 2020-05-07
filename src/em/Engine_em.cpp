#include "Engine.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

namespace cobbletext::em {

void addTextWrapper(Engine & engine, std::string text, Encoding encoding) {
    engine.addText(reinterpret_cast<const uint8_t*>(text.c_str()), text.size(), encoding);
}

void addTextUTF8Wrapper(Engine & engine, std::string text) {
    engine.addTextUTF8(text);
}

void addTextUTF16Wrapper(Engine & engine, std::wstring text) {
    engine.addTextUTF16(std::u16string(text.begin(), text.end()));
}

void addTextUTF32Wrapper(Engine & engine, std::vector<char32_t> text) {
    engine.addTextUTF32(std::u32string(text.begin(), text.end()));
}

}


EMSCRIPTEN_BINDINGS(cobbletext_Engine) {
    class_<cobbletext::Engine>("Engine")
        .property("lineLength", &cobbletext::Engine::lineLength)
        .property("locale", &cobbletext::Engine::locale)
        .property("textAlignment", &cobbletext::Engine::textAlignment)
        .property("language", &cobbletext::Engine::language)
        .property("script", &cobbletext::Engine::script)
        .property("scriptDirection", &cobbletext::Engine::scriptDirection)
        .property("font", &cobbletext::Engine::font)
        .property("fontSize", &cobbletext::Engine::fontSize)
        .property("customProperty", &cobbletext::Engine::customProperty)
        .property("outputInfo", &cobbletext::Engine::outputInfo)

        .function("tiles", &cobbletext::Engine::tiles)
        .function("advances", &cobbletext::Engine::advances)

        // .constructor<std::shared_ptr<cobbletext::Library>>()
        // .smart_ptr<std::shared_ptr<cobbletext::Engine>>("Engine")
        .smart_ptr_constructor("Engine", &std::make_shared<cobbletext::Engine,std::shared_ptr<cobbletext::Library>>)

        .function("addText", &cobbletext::em::addTextWrapper)
        .function("addTextUTF8", &cobbletext::em::addTextUTF8Wrapper)
        .function("addTextUTF16", &cobbletext::em::addTextUTF16Wrapper)
        .function("addTextUTF32", &cobbletext::em::addTextUTF32Wrapper)
        .function("addInlineObject", &cobbletext::Engine::addInlineObject)
        .function("clear", &cobbletext::Engine::clear)
        .function("layOut", &cobbletext::Engine::layOut)
        .function("tilesValid", &cobbletext::Engine::tilesValid)
        .function("rasterize", &cobbletext::Engine::rasterize)
        .function("packTiles", &cobbletext::Engine::packTiles)
        .function("clearTiles", &cobbletext::Engine::clearTiles)

        ;

    register_vector<cobbletext::TileInfo>("vector_TileInfo");
    register_vector<cobbletext::AdvanceInfo>("vector_AdvanceInfo");
    register_vector<char32_t>("vector_char32_t");
}
