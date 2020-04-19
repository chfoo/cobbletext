#include "c/engine_c.hpp"

#include "c/util_c.hpp"
#include "c/library_c.hpp"
#include "internal/Debug.hpp"

CobbletextEngine * cobbletext_engine_new(CobbletextLibrary * library) {
    CobbletextEngine * handle;

    try {
        handle = new CobbletextEngine();
        cobbletext::c::handleSuccess(library);
        handle->obj = std::make_unique<cobbletext::Engine>(library->obj);
        handle->library = library;
    } catch (std::exception & exception) {
        cobbletext::c::handleException(library, &exception);
        return nullptr;
    }

    return handle;
}

void cobbletext_engine_delete(CobbletextEngine * engine) {
    delete engine;
}

uint32_t cobbletext_engine_get_line_length(CobbletextEngine * engine) {
    return engine->obj->lineLength;
}

void cobbletext_engine_set_line_length(CobbletextEngine * engine,
        uint32_t length) {
    engine->obj->lineLength = length;
}

const char * cobbletext_engine_get_locale(CobbletextEngine * engine) {
    return engine->obj->locale.c_str();
}

void cobbletext_engine_set_locale(CobbletextEngine * engine,
        const char * locale) {
    engine->obj->locale = locale;
}

const char * cobbletext_engine_get_language(CobbletextEngine * engine) {
    return engine->obj->language.c_str();
}

void cobbletext_engine_set_language(CobbletextEngine * engine,
        const char * language) {
    engine->obj->language = language;
}

const char * cobbletext_engine_get_script(CobbletextEngine * engine) {
    return engine->obj->script.c_str();
}

void cobbletext_engine_set_script(CobbletextEngine * engine,
        const char * script) {
    engine->obj->script = script;
}

CobbletextScriptDirection cobbletext_engine_get_script_direction(
        CobbletextEngine * engine) {
    return static_cast<CobbletextScriptDirection>(engine->obj->scriptDirection);
}

void cobbletext_engine_set_script_direction(CobbletextEngine * engine,
        CobbletextScriptDirection direction) {

    cobbletext::ScriptDirection directionEnum;

    switch (direction) {
        case COBBLETEXT_SCRIPT_DIRECTION_NOT_SPECIFIED:
            directionEnum = cobbletext::ScriptDirection::NotSpecified;
            break;
        case COBBLETEXT_SCRIPT_DIRECTION_LTR:
            directionEnum = cobbletext::ScriptDirection::LTR;
            break;
        case COBBLETEXT_SCRIPT_DIRECTION_RTL:
            directionEnum = cobbletext::ScriptDirection::RTL;
            break;
        default:
            cobbletext::internal::Debug::abort("unknown script direction");
    }

    engine->obj->scriptDirection = directionEnum;
}

CobbletextFontID cobbletext_engine_get_font(CobbletextEngine * engine) {
    return engine->obj->font;
}

void cobbletext_engine_set_font(CobbletextEngine * engine,
        CobbletextFontID font) {
    engine->obj->font = font;
}

double cobbletext_engine_get_font_size(CobbletextEngine * engine) {
    return engine->obj->fontSize;
}

void cobbletext_engine_set_font_size(CobbletextEngine * engine, double size) {
    engine->obj->fontSize = size;
}

CobbletextCustomPropertyID cobbletext_engine_get_custom_property(
        CobbletextEngine * engine) {
    return engine->obj->customProperty;
}

void cobbletext_engine_set_custom_property(CobbletextEngine * engine,
        CobbletextCustomPropertyID id) {
    engine->obj->customProperty = id;
}

void cobbletext_engine_add_text(CobbletextEngine * engine,
        const uint8_t * data, int32_t length, CobbletextEncoding encoding) {

    cobbletext::Encoding encodingEnum;

    switch (encoding) {
        case COBBLETEXT_ENCODING_UTF8:
            encodingEnum = cobbletext::Encoding::UTF8;
            break;
        case COBBLETEXT_ENCODING_UTF16LE:
            encodingEnum = cobbletext::Encoding::UTF16LE;
            break;
        default:
            cobbletext::internal::Debug::abort("unknown encoding type");
    }

    engine->obj->addText(data, length, encodingEnum);
}

void cobbletext_engine_add_text_utf8(CobbletextEngine * engine, const char * text, int32_t length) {
    engine->obj->addTextUTF8(text, length);
}

void cobbletext_engine_add_text_utf16(CobbletextEngine * engine, const char16_t * text, int32_t length) {
    engine->obj->addTextUTF16(text, length);
}

void cobbletext_engine_add_text_utf32(CobbletextEngine * engine, const char32_t * text, int32_t length) {
    engine->obj->addTextUTF32(text, length);
}

void cobbletext_engine_add_inline_object(CobbletextEngine * engine, CobbletextInlineObjectID id, uint32_t size) {
    engine->obj->addInlineObject(id, size);
}

void cobbletext_engine_clear(CobbletextEngine * engine) {
    engine->obj->clear();
}

void cobbletext_engine_lay_out(CobbletextEngine * engine) {
    try {
        engine->obj->layOut();
        cobbletext::c::handleSuccess(engine->library);
    } catch (std::exception & exception) {
        cobbletext::c::handleException(engine->library, &exception);
        return;
    }
    engine->tilesPrepared = engine->advancesPrepared = false;
}

bool cobbletext_engine_tiles_valid(CobbletextEngine * engine) {
    return engine->obj->tilesValid();
}

void cobbletext_engine_rasterize(CobbletextEngine * engine) {
    engine->obj->rasterize();
}

uint32_t cobbletext_engine_get_image_width(CobbletextEngine * engine) {
    return engine->obj->imageWidth;
}

uint32_t cobbletext_engine_get_image_height(CobbletextEngine * engine) {
    return engine->obj->imageHeight;
}

bool cobbletext_engine_pack_tiles(CobbletextEngine * engine, uint32_t width,
        uint32_t height) {
    return engine->obj->packTiles(width, height);
}

void cobbletext_engine_prepare_tiles(CobbletextEngine * engine) {
    try {
        cobbletext::c::prepareEngineTiles(engine);
        engine->tilesPrepared = true;
        cobbletext::c::handleSuccess(engine->library);
    } catch (std::exception & exception) {
        cobbletext::c::handleException(engine->library, &exception);
    }
}

void cobbletext_engine_prepare_advances(CobbletextEngine * engine) {
    try {
        cobbletext::c::prepareEngineAdvances(engine);
        engine->advancesPrepared = true;
        cobbletext::c::handleSuccess(engine->library);
    } catch (std::exception & exception) {
        cobbletext::c::handleException(engine->library, &exception);
    }
}

uint32_t cobbletext_engine_get_tile_count(CobbletextEngine * engine) {
    if (!engine->tilesPrepared) {
        cobbletext::internal::Debug::abort("Tiles not prepared");
    }

    return engine->tiles.size();
}

const struct CobbletextTileInfo ** cobbletext_engine_get_tiles(
        CobbletextEngine * engine) {
    if (!engine->tilesPrepared) {
        cobbletext::internal::Debug::abort("Tiles not prepared");
    }

    return engine->tilesPointer.get();
}

uint32_t cobbletext_engine_get_advance_count(CobbletextEngine * engine) {
    if (!engine->advancesPrepared) {
        cobbletext::internal::Debug::abort("Advances not prepared");
    }

    return engine->advances.size();
}

const struct CobbletextAdvanceInfo ** cobbletext_engine_get_advances(
        CobbletextEngine * engine) {
    if (!engine->advancesPrepared) {
        cobbletext::internal::Debug::abort("Advances not prepared");
    }

    return engine->advancesPointer.get();
}


namespace cobbletext::c {

void prepareEngineTiles(CobbletextEngine * engine) {
    engine->tiles.clear();
    auto tiles = engine->obj->tiles();

    auto tilesPointer = new const struct CobbletextTileInfo * [tiles.size()];
    engine->tilesPointer.reset(tilesPointer);

    for (size_t index = 0; index < tiles.size(); index++) {
        struct CobbletextTileInfo * cTileInfo = new struct CobbletextTileInfo;
        auto & tile = tiles[index];

        cTileInfo->atlas_x = tile.atlasX;
        cTileInfo->atlas_y = tile.atlasY;
        cTileInfo->glyph_id = tile.glyphID;

        engine->tiles.push_back(
            std::unique_ptr<const struct CobbletextTileInfo>(cTileInfo));
        engine->tilesPointer.get()[index] = engine->tiles[index].get();
    }
}

void prepareEngineAdvances(struct CobbletextEngine * engine) {
    engine->advances.clear();
    auto advances = engine->obj->advances();

    auto advancesPointer =
        new const struct CobbletextAdvanceInfo * [advances.size()];
    engine->advancesPointer.reset(advancesPointer);

    for (size_t index = 0; index < advances.size(); index++) {
        struct CobbletextAdvanceInfo * cAdvanceInfo =
            new struct CobbletextAdvanceInfo;
        auto & advance = advances[index];

        cAdvanceInfo->advance_x = advance.advanceX;
        cAdvanceInfo->advance_y = advance.advanceY;
        cAdvanceInfo->custom_property = advance.customProperty;
        cAdvanceInfo->glyph_id = advance.glyphID;
        cAdvanceInfo->glyph_offset_x = advance.glyphOffsetX;
        cAdvanceInfo->glyph_offset_y = advance.glyphOffsetY;
        cAdvanceInfo->inline_object = advance.inlineObject;
        cAdvanceInfo->text_index = advance.textIndex;
        cAdvanceInfo->type = static_cast<CobbletextAdvanceType>(advance.type);

        engine->advances.push_back(
            std::unique_ptr<const struct CobbletextAdvanceInfo>(cAdvanceInfo));
        engine->advancesPointer.get()[index] = engine->advances[index].get();
    }
}

}
