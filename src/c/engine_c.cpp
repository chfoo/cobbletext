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

const struct CobbletextEngineProperties * cobbletext_engine_get_properties(
        CobbletextEngine * engine) {
    if (!engine->properties) {
        auto properties = new struct CobbletextEngineProperties;
        engine->properties.reset(properties);
    }

    engine->properties->line_length = engine->obj->lineLength;
    engine->properties->locale = engine->obj->locale.c_str();

    return engine->properties.get();
}


void cobbletext_engine_set_properties(CobbletextEngine * engine,
        const struct CobbletextEngineProperties * properties) {

    engine->obj->lineLength = properties->line_length;
    engine->obj->locale = std::string(properties->locale ?
        properties->locale : "");
}

const struct CobbletextTextProperties * cobbletext_engine_get_text_properties(
        CobbletextEngine * engine) {
    if (!engine->textProperties) {
        auto textProperties = new struct CobbletextTextProperties;
        engine->textProperties.reset(textProperties);
    }

    engine->textProperties->language = engine->obj->language.c_str();
    engine->textProperties->script = engine->obj->script.c_str();
    engine->textProperties->script_direction =
        static_cast<CobbletextScriptDirection>(engine->obj->scriptDirection);
    engine->textProperties->font = engine->obj->font;
    engine->textProperties->font_size = engine->obj->fontSize;
    engine->textProperties->custom_property = engine->obj->customProperty;

    return engine->textProperties.get();
}


void cobbletext_engine_set_text_properties(CobbletextEngine * engine,
        const struct CobbletextTextProperties * text_properties) {

    engine->obj->language = std::string(text_properties->language ?
        text_properties->language : "");
    engine->obj->script = std::string(text_properties->script ?
        text_properties->script : "");

    cobbletext::ScriptDirection directionEnum;

    switch (text_properties->script_direction) {
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

    engine->obj->font = text_properties->font;
    engine->obj->fontSize = text_properties->font_size;
    engine->obj->customProperty = text_properties->custom_property;
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
    try {
        engine->obj->rasterize();
        cobbletext::c::handleSuccess(engine->library);
    } catch (std::exception & exception) {
        cobbletext::c::handleException(engine->library, &exception);
    }
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

const struct CobbletextOutputInfo * cobbletext_engine_get_output_info(
        CobbletextEngine * engine) {

    struct CobbletextOutputInfo * info = new struct CobbletextOutputInfo();

    engine->outputInfo.reset(info);

    info->text_width = engine->obj->outputInfo.textWidth;
    info->text_height = engine->obj->outputInfo.textHeight;

    return info;
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
