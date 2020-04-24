# sdl_example

This C++ program uses SDL to display the text. It draws all the glyphs to a texture atlas (with data on the alpha channel) and renders sections of the texture to the renderer.

While the glyphs are rendered with alpha blending in this example, gamma correction is not performed to improve readability due to limitations in the SDL API.

For high performance rendering in 3D applications, an array of tile information can be passed to a shader program that draws the texture atlas with alpha blending and gamma correction. This is out of scope for this example.

Example usage:

    ./sdl_example -f /usr/share/fonts/truetype/noto/NotoSans-Regular.ttf \
        -f /usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc \
        -f /usr/share/fonts/truetype/unifont/unifont.ttf

Use `./sdl_example -h` to see all options.
