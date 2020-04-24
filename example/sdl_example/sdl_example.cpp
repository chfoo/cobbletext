#include "sdl_example.hpp"

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <cassert>

#include "../common/sample_text.h"

int main(int argc, char *argv[]) {
    std::cerr << "Start" << std::endl;

    int errorCode = SDL_Init(SDL_INIT_VIDEO);

    if (errorCode) {
        std::cerr << SDL_GetError() << std::endl;
        std::exit(1);
    }

    example::App app;
    app.parseArgs(argc, argv);
    app.run();

    SDL_Quit();

    std::cerr << "Done" << std::endl;
}

namespace example {

App::App() :
        library(std::make_shared<cobbletext::Library>()),
        engine(std::make_shared<cobbletext::Engine>(library)) {

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetSwapInterval(1);

    window.reset(SDL_CreateWindow("Cobbletext demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_RESIZABLE));

    if (!window) {
        reportSDLError();
    }

    renderer.reset(SDL_CreateRenderer(window.get(), -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if (!renderer) {
        reportSDLError();
    }
}

void App::parseArgs(int argc, char * argv[]) {
    while (true) {
        int option = getopt(argc, argv, "hf:a:l:");

        if (option < 0) {
            break;
        }

        switch (option) {
            case 'h':
                printHelp();
                std::exit(0);
                break;
            case 'f':
                loadFont(optarg);
                break;
            case 'a': {
                uint32_t textAlignment = strtoumax(optarg, NULL, 0);
                engine->textAlignment =
                    static_cast<cobbletext::TextAlignment>(textAlignment);
                break;
            }
            case 'l':
                engine->locale = optarg;
                break;
            case '?':
                std::exit(2);
        }
    }
}

void App::checkSDLError(int errorCode) {
    if (errorCode) {
        reportSDLError();
    }
}

void App::reportSDLError() {
    std::cerr << SDL_GetError() << std::endl;
    std::exit(1);
}

void App::printHelp() {
    std::cerr <<
        "Usage: ppm_example [-l LOCALE] [-w LINE_LENGTH] [-a TEXT_ALIGNMENT] [-f FONT_FILE]...\n"
        "-h help\n"
        "-l BCP 47 language tag\n"
        "-w Positive value in pixels for word wrapping. 0 for none.\n"
        "-a Number of {0, 1, 2, 3, 4, 5} to select (not specified, start, end, left, right, center)\n"
        "-f Path to a font file name. Use multiple -f for font fallback. \n";
}

void App::run() {
    running = true;

    setUpFontFallback();
    setUpText();

    checkSDLError(SDL_SetRenderDrawBlendMode(
        renderer.get(), SDL_BLENDMODE_BLEND));

    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            processEvent(event);
        }

        if (windowDirty) {
            checkSDLError(SDL_SetRenderDrawColor(renderer.get(),
                255,255, 255, 255));
            SDL_RenderClear(renderer.get());
            layOutText();
            drawText();
            SDL_RenderPresent(renderer.get());
            windowDirty = false;
        }
    }
}

void App::processEvent(SDL_Event & event) {
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    windowDirty = true;
                    break;
            }
            break;
    }
}

void App::loadFont(char * path) {
    std::cerr << "Loading font " << path << std::endl;

    auto fontID = library->loadFont(path);
    fonts.push_back(fontID);
}

void App::setUpFontFallback() {
    for (size_t index = 0; index < fonts.size(); index++) {
        if (index < fonts.size() - 1) {
            std::cerr << "Setting font alternative font "
                << fonts[index] << "->" << fonts[index + 1] << std::endl;

            library->setFontAlternative(fonts[index], fonts[index + 1]);
        }
    }
}

void App::setUpText() {
    std::cerr << "Adding text and objects.." << std::endl;

    engine->fontSize = 16;
    engine->customProperty = static_cast<cobbletext::CustomPropertyID>(
        CustomProperty::None);

    if (!fonts.empty()) {
        engine->font = fonts[0];
    }

    engine->addTextUTF8(EAT_GLASS_TEXT);
    engine->addTextUTF8("\n");

    engine->fontSize = 10;
    engine->customProperty = static_cast<cobbletext::CustomPropertyID>(
        CustomProperty::RedText);

    engine->addTextUTF8("Inline");
    engine->addInlineObject(InlineObject1, 50);
    engine->addTextUTF8("object");

    engine->fontSize = 16;
    engine->customProperty = static_cast<cobbletext::CustomPropertyID>(
        CustomProperty::None);
    engine->addTextUTF8(EMOJI_TEXT);
}

void App::layOutText() {

    int rendererWidth;
    int rendererHeight;

    checkSDLError(
        SDL_GetRendererOutputSize(renderer.get(),
        &rendererWidth, &rendererHeight));

    engine->lineLength = rendererWidth;
    engine->layOut();

    if (!engine->tilesValid()) {
        createAtlas();
    }
}

void App::createAtlas() {
    std::cerr << "Rasterizing..." << std::endl;
    engine->rasterize();

    std::cerr << "Creating atlas..." << std::endl;
    packTilesForAtlas();

    atlas.reset(SDL_CreateTexture(renderer.get(),
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
        atlasSize, atlasSize));

    if (!atlas) {
        reportSDLError();
    }

    void * textureData;
    int pitch;
    checkSDLError(SDL_LockTexture(atlas.get(), nullptr, &textureData, &pitch));

    uint8_t * atlasData = reinterpret_cast<uint8_t *>(textureData);

    auto tiles = engine->tiles();

    for (auto & tile : tiles) {
        auto glyph = library->getGlyphInfo(tile.glyphID);

        drawAtlasTile(atlasData, tile, glyph);

        AtlasEntry entry;
        entry.atlasX = tile.atlasX;
        entry.atlasY = tile.atlasY;
        entry.imageWidth = glyph.imageWidth;
        entry.imageHeight = glyph.imageHeight;
        entry.imageOffsetX = glyph.imageOffsetX;
        entry.imageOffsetY = glyph.imageOffsetY;
        atlasTable[tile.glyphID] = entry;
    }

    SDL_UnlockTexture(atlas.get());

    checkSDLError(SDL_SetTextureBlendMode(atlas.get(), SDL_BLENDMODE_BLEND));
}

void App::packTilesForAtlas() {
    while (true) {
        bool hasOverflow = engine->packTiles(atlasSize, atlasSize);

        if (!hasOverflow) {
            break;
        }

        atlasSize *= 2;
    }
}

void App::drawAtlasTile(uint8_t * atlasData,
        const cobbletext::TileInfo & tile,
        const cobbletext::GlyphInfo & glyph) {

    size_t tileSize = glyph.imageWidth * glyph.imageHeight;

    for (size_t index = 0; index < tileSize; index++) {
        size_t glyphImageX = index % glyph.imageWidth;
        size_t glyphImageY = index / glyph.imageWidth;
        size_t atlasX = tile.atlasX + glyphImageX;
        size_t atlasY = tile.atlasY + glyphImageY;
        size_t atlasIndex = atlasSize * atlasY + atlasX;
        size_t atlasDataIndex = atlasIndex * 4;
        uint8_t coverage = glyph.image[index];

        // #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        // atlasData[atlasDataIndex + 0] = 0xff;
        // atlasData[atlasDataIndex + 1] = 0xff;
        // atlasData[atlasDataIndex + 2] = 0xff;
        // atlasData[atlasDataIndex + 3] = coverage;
        // #else
        atlasData[atlasDataIndex + 0] = 0xff;
        atlasData[atlasDataIndex + 1] = 0xff;
        atlasData[atlasDataIndex + 2] = 0xff;
        atlasData[atlasDataIndex + 3] = coverage;
        // #endif
    }
}

void App::drawText() {
    penX = penY = 0;

    auto advances = engine->advances();

    for (auto & advance : advances) {
        switch (advance.type) {
            case cobbletext::AdvanceType::Glyph:
                drawGlyph(advance);
        }

        penX += advance.advanceX;
        penY += advance.advanceY;
    }
}

void App::drawGlyph(const cobbletext::AdvanceInfo & advance) {
    auto & atlasEntry = atlasTable.at(advance.glyphID);

    SDL_Rect sourceRect;
    sourceRect.x = atlasEntry.atlasX;
    sourceRect.y = atlasEntry.atlasY;
    sourceRect.w = atlasEntry.imageWidth;
    sourceRect.h = atlasEntry.imageHeight;

    SDL_Rect destinationRect;
    destinationRect.x = penX + atlasEntry.imageOffsetX + advance.glyphOffsetX;
    destinationRect.y = penY + atlasEntry.imageOffsetY + advance.glyphOffsetY;
    destinationRect.w = atlasEntry.imageWidth;
    destinationRect.h = atlasEntry.imageHeight;

    switch (static_cast<CustomProperty>(advance.customProperty)) {
        case CustomProperty::None:
            checkSDLError(SDL_SetTextureColorMod(atlas.get(), 0, 0, 0));
            break;
        case CustomProperty::RedText:
            checkSDLError(SDL_SetTextureColorMod(atlas.get(), 255, 0, 0));
            break;
    }

    checkSDLError(SDL_RenderCopy(renderer.get(), atlas.get(),
        &sourceRect, &destinationRect));
}

}




