# Cobbletext

Cobbletext is a complex text layout and rendering engine as a C/C++ library. It bundles formatting, bidirectional analysis, shaping, line breaking, and glyph rasterization into one simple interface. It is not tied to any specific operating system framework and is designed for general purpose use on graphics contexts such as OpenGL, SDL, or HTML5 Canvas.

It uses FreeType, HarfBuzz, and Unicode ICU for the actual dirty work.

Online demos:

* [js_example](https://chfoo.github.io/cobbletext/example/js_example/) JavaScript with Emscripten bindings. Note: requires 30 MB of data download due to fonts.

## Quick start

See [INSTALL.md](INSTALL.md) for building the library.

For prebuilt libraries, check the Releases section in the GitHub repository. Prebuilt libraries might not always work. Check the troubleshooting section for details.

On macOS, please define `COBBLETEXT_NO_UCHAR_H`.

### Prepare context

Before any text processing can be done, a library context instance needs to be created:

```c++
// C
#include <cobbletext/cobbletext.h>
CobbletextLibrary library * = cobbletext_new_library();

// C++
#include <cobbletext/cobbletext.hpp>
auto library = std::make_shared<cobbletext::Library>();
```

Only one library context is required. However, for threading situations, a library context can be created per thread since Cobbletext and its dependencies is not thread-safe.

### Error handling

Whenever there is an error, an exception is thrown. On C, the error can be retrieved like so:

```c++
int32_t error_code = cobbletext_get_error_code(library);
const char * error_message = cobbletext_get_error_message(library);
```

This tutorial will omit error handling for brevity.

### Loading fonts

To draw any useful text, a font file must be specified:

```c++
// C
CobbletextFontID myFont = cobbletext_library_load_font(library, "path_to_font.ttf");

// C++
cobbletext::FontID myFont = library->loadFont("path_to_font.ttf");
```

Each font face needs to be loaded individually. The returned ID allows you to select that font later. (At this time, it's not possible to select loaded fonts by name and style.)

### Create engine

A layout engine is created like so:

```c++
// C
CobbletextEngine * engine = cobbletext_engine_new(library);

// C++
auto engine = cobbletext::Engine(library);
```

An engine is used repeatedly to process text. It holds a text buffer and properties for runs of text.

Multiple engines can be created for specific purposes. For example, one engine can be dedicated for the application GUI elements and another engine can be dedicated to chat room messages.

### Set engine properties

Once the engine is created, you can apply some settings that control its output:

```c++
// C
struct CobbletextEngineProperties properties = {0};
properties.line_length = 500;
properties.locale = "en-US";
cobbletext_engine_set_properties(engine, &properties);

// C++
engine.lineLength = 500;
engine.locale = "en-US";
```

### Set text properties

Before adding text, set some basic text formatting. These properties will be applied to the subsequent runs of text:

```c++
// C
struct CobbletextTextProperties text_properties = {0};
text_properties.font = myFont;
text_properties.font_size = 16;
cobbletext_engine_set_text_properties(engine, &text_properties);

// C++
engine.font = myFont;
engine.fontSize = 16;
```

### Adding text

Finally, we can add some text:

```c++
// C
cobbletext_engine_add_text_utf8(engine, "Hello world!", -1);

// C++
engine.addTextUTF8("Hello world!");
```

### Do lay out

Once all the text has been added, we can perform processing of the text:

```c++
// C
cobbletext_engine_lay_out(engine);

// C++
engine.layOut();
```

### Process tiles

Now the text has been processed, the results are stored as tiles and advances. Tiles represent images of each glyphs associated to the given text and advances represent the pen instructions to draw tiles.

First, we'll convert the glyphs to images:

```c++
// C
cobbletext_engine_rasterize(engine);

// C++
engine.rasterize();
```

Then, we'll arrange the tiles into a texture atlas:

```c++
// C
bool has_overflow = cobbletext_engine_pack_tiles(engine, 256, 256);

// C++
bool hasOverflow = engine.packTiles(256, 256);
```

The tile packing function will return a value indicating that the tiles did not fit within the texture. To handle the case where it does not fit, double the texture size. If you aren't using a texture atlas, you can skip the texture atlas step.

Now get the tiles:

```c++
// C
cobbletext_engine_prepare_tiles(engine);
uint32_t tile_count = cobbletext_engine_get_tile_count(engine);
const struct CobbletextTileInfo ** tile_array cobbletext_engine_get_tiles(engine);

// C++
std::vector<TileInfo> tiles = engine.tiles();
```

Draw the images to the texture atlas, and store the atlas metadata to a hash table:

```c++
// C
for (uint32_t index; index < tile_count; index++) {
    const struct CobbletextTileInfo * tile = tile_array[index];
    const struct CobbletextGlyphInfo * glyph = cobbletext_library_get_glyph_info(library, tile->glyph_id);

    my_draw_to_atlas(
        glyph->image, glyph->image_width, glyph->image, // Source image data
        tile->atlas_x, tile->atlas_y // Destination on texture
    );
    my_atlas_table_set(
        tile->glyph_id, // Key
        tile->atlas_x, tile->atlas_y, // Location on atlas
        glyph->image_offset_x, glyph->image_offset_y // Drawing metadata
    );
}

// C++
for (auto & tile : tiles) {
    auto glyph = library.getGlyphInfo(tile.glyphID);

    myDrawToAtlas(
        glyph.image, glyph.imageWidth, glyph.imageHeight, // Source
        tile.atlasX, tile.atlasY // Destination
    );
    myAtlas.set(
        tile.glyphID, // Key
        tile.atlasX, tile.atlasY, // Location on atlas
        glyph.imageOffsetX, glyph.imageOffsetY // Drawing metadata
    );
}
```

In order to not process image data to every time text is changed, use:

```c++
// C
bool isValid = cobbletext_engine_tiles_valid(engine);

// C++
bool isValid = engine.tilesValid();
```

For a texture atlas, this function can be used to determine whether a texture atlas should be rebuilt with new additional tiles. If the tiles are not valid, then:

1. Clear texture.
2. Call "rasterize" function.
3. Call "pack tiles" function.
4. Process tiles by adding or replacing tile metadata.
5. Draw the glyphs to the texture.

### Prepare destination image

Then we prepare a destination image:

```c++
// C
const struct CobbletextOutputInfo * output_info = cobbletext_engine_get_output_info(engine);
MyImage * image = my_make_image(output_info->text_width, output_info->text_height);

// C++
auto image = MyImage(engine.outputInfo.textWidth, engine.outputInfo.textHeight);
```

### Drawing advances

Now we can draw the text. First we get the advances:

```c++
// C
cobbletext_prepare_advances(engine);
uint32_t advance_count = cobbletext_engine_get_advance_count()
const struct CobbletextAdvanceInfo ** advances_array = cobbletext_engine_get_advances(engine);

// C++
std::vector<cobbletext::AdvanceInfo> advances = engine.advances();
```

Then iterate through each advance, drawing glyphs when needed:

```c++
// C
int32_t pen_x = 0;
int32_t pen_y = 0;

for (int32_t index; index < advance_count; index++) {
    const struct CobbletextAdvanceInfo * advance = advances_array[index];

    switch (advance->type) {
        case COBBLETEXT_ADVANCE_TYPE_GLYPH:
            MyAtlasEntry * entry = my_atlas_table_get(advance->glyph_id);

            int32_t x = pen_x + advance->glyph_offset_x + entry->image_offset_x;
            int32_t y = pen_y + advance->glyph_offset_x + entry->image_offset_x;

            my_image_draw_tile(image,
                entry->atlas_x, entry->atlas_y, // Source position
                x, y // Destination position
            );
            break;
    }

    pen_x += advance->advance_x;
    pen_y += advance->advance_y;
}

// C++
int32_t penX = 0;
int32_t penY = 0;

for (auto & advance : advances) {
    switch (advance->type) {
        case cobbletext::AdvanceType::Glyph:
            auto & entry = myAtlas.get(advance->glyphID);

            int32_t x = penX + advance->glyphOffsetX + entry->imageOffsetX;
            int32_t y = penY + advance->glyphOffsetY + entry->imageOffsetX;

            image.myDrawTile(
                entry->atlasX, entry->atlasY, // Source position
                x, y // Destination position
            );
            break;
    }

    penX += advance->advanceX;
    penY += advance->advanceY;
}
```

The text is finally rendered!

### Reuse engine

To reuse the engine, use the clear function which will remove the text from its internal buffer, but keep all properties and tiles intact:

```c++
// C
cobbletext_engine_clear(engine);

// C++
engine.clear();
```

### Clearing tiles

If your texture atlas for an engine is filling up, instead of deleting and recreating an engine entirely, you can clear the tiles and the associated glyphs.

```c++
// C
cobbletext_engine_clear_tiles(engine);

// C++
engine.clearTiles();
```

Then, you proceed as if you have a blank texture atlas by creating a new texture and atlas metadata hash table, or clear the texture and hash table.

## Troubleshooting libraries

If you have trouble getting the libraries to work at runtime, ensure the libraries are located on the OS's search path. As well ensure the library's dependencies are also working.

### Windows

Libraries can be placed in the same directory as the executable. If library refuses to load despite it being there, check that its dependencies are found using Dependency Walker or Process Monitor. You might need the latest Visual C++ Redistributable.

### MacOS

Placing libraries in the same directory as the executable might not work. You can temporarily specify a search path using the `DYLD_FALLBACK_LIBRARY_PATH` environment variable. For example: `DYLD_FALLBACK_LIBRARY_PATH=my/path/to/dylib/directory/:/usr/local/lib:/lib:/usr/lib ./my_application`. Note that macOS strips this environment variable on child processes so you can't `export` it.

Use `otool` to inspect your application's library dependencies. The libraries may be linked into a hardcoded path which may not exist on another system. Or the libraries use `@rpath/` prefix but your application does not specify an rpath. Use `install_name_tool` to change the names.

### Linux

You can temporarily include library paths using the `LD_LIBRARY_PATH` environment variable. For example: `LD_LIBRARY_PATH=my/path/to/so/directory/ ./my_application`. The `ldd` command and setting the environment variable `LD_DEBUG=libs` can be useful.

## Further reading

* Please see the examples in the example directory.
* [API documentation](https://chfoo.github.io/cobbletext/doc/api/)
* [HarfBuzz documentation](https://harfbuzz.github.io/index.html)
* [FreeType documentation](https://www.freetype.org/freetype2/docs/documentation.html)
* [ICU documentation](http://userguide.icu-project.org/)

## Supported features

| Feature | Is supported? |
|---------|---------------|
| Text transform (capitalization) | No |
| Line breaking (mandatory, word wrapping) | Yes |
| Alignment | Yes |
| Justification | No |
| Bidirectional text | Yes |
| Vertical text | No |
| Font name matching (family, style, weight, etc.) | No |
| Font fallback | Yes |
| Language-specific display | Yes |
| Kerning | Yes |
| Ligatures | Yes |
| Subscript & superscript | No |
| Capitalization, numerical formatting | No |
| Ruby | No |
| Line decoration (underline, overline, strike-through) | No |
| Emphasis marks | No |
| Cursor positioning by index | Yes |
| Cursor positioning by XY-coordinate | No |

## Contributing

If you have any issues or improvements, please use the GitHub Issues and Pull Requests section.

## License

Copyright 2020 Christopher Foo. Licensed under Mozilla Public License Version 2.0.

Dependencies used within the library:

* Microsoft GSL: MIT license
* Boost: BSD-like license
* STB: MIT license
* FreeType: BSD-like license or GPLv2
  * Bzip2/libbz2: BSD-like license
  * Libpng: BSD-like license
  * zlib: BSD-like license
* HarfBuzz: MIT license
* ICU/ICU4C: BSD-like license
* Adobe NotDef: SIL Open Font license
