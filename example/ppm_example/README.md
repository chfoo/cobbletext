# ppm_example

This C program writes a PPM image file output along with a PGM image of each glyph.

Example usage:

    ./ppm_example -w 500 -f /usr/share/fonts/truetype/noto/NotoSans-Regular.ttf \
        -f /usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc \
        -f /usr/share/fonts/truetype/unifont/unifont.ttf

Use `./ppm_example -h` to see all options.
