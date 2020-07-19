# js_example

This HTML app shows drawing glyphs using the HTML5 canvas and cobbletext.js.

Note that the HTML5 2D graphics context doesn't work entirely in our favor, so the example only supports colors on the entire text. As well, gamma correction for improved readability has been omitted.

To run the example, it requires the files:

* cobbletext.js
* cobbletext.wasm
* NotoSans-Regular.ttf
* NotoSansCJK-Regular.ttc
* NotoSansArabic-Regular.ttf
* NotoSansHebrew-Regular.ttf
* Symbola.otf

Some of the API returns handles to C++ vectors. These need to be deleted manually from the JavaScript side. In a future release, this may be improved to use regular JavaScript arrays.
