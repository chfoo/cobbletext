#include "c/library_c.hpp"



#include "LibraryImpl.hpp"
#include "Exception.hpp"



CobbletextLibrary * cobbletext_library_new() {
    auto handle = new struct CobbletextLibrary();

    try {
        handle->obj = std::make_shared<cobbletext::Library>();
        cobbletext::c::handleSuccess(handle);
    } catch (std::exception & exception) {
        cobbletext::c::handleException(handle, &exception);
    }

    return handle;
}

void cobbletext_library_delete(CobbletextLibrary * library) {
    delete library;
}

int32_t cobbletext_get_error_code(CobbletextLibrary * library) {
    return library->errorCode;
}

const char * cobbletext_get_error_message(CobbletextLibrary * library) {
    return library->errorMessage.c_str();
}

void cobbletext_clear_error(CobbletextLibrary * library) {
    library->errorCode = 0;
    library->errorMessage = "";
}

CobbletextFontID cobbletext_library_get_fallback_font(
        CobbletextLibrary * library) {
    return library->obj->fallbackFont();
}

CobbletextFontID cobbletext_library_load_font(CobbletextLibrary * library,
        const char * path) {
    try {
        auto fontID = library->obj->loadFont(path);
        cobbletext::c::handleSuccess(library);
        return fontID;
    } catch (std::exception & exception) {
        cobbletext::c::handleException(library, &exception);
        return 0;
    }
}

CobbletextFontID cobbletext_library_load_font_bytes(CobbletextLibrary * library,
        const uint8_t * data, uint32_t length) {
    try {
        auto fontID = library->obj->loadFontBytes(data, length);
        cobbletext::c::handleSuccess(library);
        return fontID;
    } catch (std::exception & exception) {
        cobbletext::c::handleException(library, &exception);
        return 0;
    }
}

const struct CobbletextFontInfo * cobbletext_library_get_font_info(
        CobbletextLibrary * library, CobbletextFontID font) {

    cobbletext::FontInfo fontInfo;

    try {
        fontInfo = library->obj->getFontInfo(font);
        cobbletext::c::handleSuccess(library);

    } catch (std::exception & exception) {
        cobbletext::c::handleException(library, &exception);
        return nullptr;
    }

    library->fontInfo = std::make_unique<CobbletextFontInfo>();
    library->fontInfo->ascender = fontInfo.ascender;
    library->fontInfo->descender = fontInfo.descender;
    library->fontInfo->family_name = fontInfo.familyName.c_str();
    library->fontInfo->height = fontInfo.height;
    library->fontInfo->id = fontInfo.id;
    library->fontInfo->style_name = fontInfo.styleName.c_str();
    library->fontInfo->underline_position = fontInfo.underlinePosition;
    library->fontInfo->underline_thickness = fontInfo.underlineThickness;
    library->fontInfo->units_per_em = fontInfo.unitsPerEM;

    return library->fontInfo.get();
}

const struct CobbletextGlyphInfo * cobbletext_library_get_glyph_info(
        CobbletextLibrary * library, CobbletextFontID glyph) {
    cobbletext::GlyphInfo glyphInfo;

    try {
        glyphInfo = library->obj->getGlyphInfo(glyph);
        cobbletext::c::handleSuccess(library);
    } catch (std::exception & exception) {
        cobbletext::c::handleException(library, &exception);
        return nullptr;
    }

    if (library->glyphInfo) {
        delete [] library->glyphInfo->image;
    }

    library->glyphInfo = std::make_unique<CobbletextGlyphInfo>();
    library->glyphInfo->id = glyphInfo.id;

    uint8_t * image = new uint8_t[glyphInfo.image.size()];
    std::copy(glyphInfo.image.begin(), glyphInfo.image.end(), image);
    library->glyphInfo->image = image;

    library->glyphInfo->image_width = glyphInfo.imageWidth;
    library->glyphInfo->image_height = glyphInfo.imageHeight;

    return library->glyphInfo.get();
}



namespace cobbletext::c {

void handleSuccess(struct CobbletextLibrary * library) {
    library->errorCode = 0;
    library->errorMessage = "";
}

void handleException(struct CobbletextLibrary * library,
        std::exception * exception) {
    auto * libraryError = dynamic_cast<cobbletext::LibraryError*>(exception);

    if (libraryError) {
        library->errorCode = libraryError->code;
        library->errorMessage = libraryError->message;
    } else {
        library->errorCode = -1;
        library->errorMessage = exception->what();
    }
}

}
