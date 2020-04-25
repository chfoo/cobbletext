#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

#define SDL_MAIN_HANDLED

#ifdef EXAMPLE_INCLUDE_SDL_STYLE_1
    #include <SDL2/SDL.h>
#elif defined(EXAMPLE_INCLUDE_SDL_STYLE_2)
    #include <SDL/SDL.h>
#else
    #include <SDL.h>
#endif

#include <cobbletext/cobbletext.hpp>

#ifdef __EMSCRIPTEN__
extern "C" {
    void EMSCRIPTEN_KEEPALIVE setRendererSize(double width, double height);
}
#endif

namespace example {

class WindowDeleter {
public:
    void operator()(SDL_Window * pointer) {
        SDL_DestroyWindow(pointer);
    }
};

class RendererDeleter {
public:
    void operator()(SDL_Renderer * pointer) {
        SDL_DestroyRenderer(pointer);
    }
};

class SurfaceDeleter {
public:
    void operator()(SDL_Surface * pointer) {
        SDL_FreeSurface(pointer);
    }
};

class TextureDeleter {
public:
    void operator()(SDL_Texture * pointer) {
        SDL_DestroyTexture(pointer);
    }
};

enum class CustomProperty : int {
    None = 0,
    RedText = 1
};

class AtlasEntry {
public:
    uint32_t atlasX;
    uint32_t atlasY;
    uint32_t imageWidth;
    uint32_t imageHeight;
    int32_t imageOffsetX;
    int32_t imageOffsetY;
};

class App {
    std::unique_ptr<SDL_Window,WindowDeleter> window;
    std::unique_ptr<SDL_Renderer,RendererDeleter> renderer;
    bool running = false;
    bool windowDirty = true;

    std::shared_ptr<cobbletext::Library> library;
    std::shared_ptr<cobbletext::Engine> engine;

    std::vector<cobbletext::FontID> fonts;

    static constexpr int InlineObject1 = 1;

    uint32_t atlasSize = 128;
    std::unique_ptr<SDL_Texture,TextureDeleter> atlas;
    std::unordered_map<cobbletext::GlyphID,AtlasEntry> atlasTable;

    int32_t penX;
    int32_t penY;

    int rendererWidth;
    int rendererHeight;

public:
    App();
    void parseArgs(int argc, char * argv[]);
    void run();

private:
    #ifdef __EMSCRIPTEN__
    static void emCallback(void* userData);
    #endif

    void checkSDLError(int errorCode);
    [[noreturn]] void reportSDLError();

    void runOnce();

    void printHelp();
    void processEvent(SDL_Event & event);
    void loadFont(char * path);
    void setUpFontFallback();
    void setUpText();
    void layOutText();
    void createAtlas();
    void packTilesForAtlas();
    void drawAtlasTile(uint8_t * textureData,
        const cobbletext::TileInfo & tile,
        const cobbletext::GlyphInfo & glyph);
    void drawText();
    void drawGlyph(const cobbletext::AdvanceInfo & advance);
};

}

