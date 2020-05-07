"use strict";

var cobbletext;
var library;
var engine;
var atlasTable = new Map();
var fontIds = [];

var loadingDiv;
var inputTextarea;
var renderCanvas;
var colorCanvas;
var atlasCanvas;
var colorInputElement;
var textSizeElement;
var localeElement;
var renderClearButton;

if (document.readyState == "loading") {
    document.addEventListener("DOMContentLoaded", documentLoadedCallback);
} else {
    documentLoadedCallback();
}

function documentLoadedCallback() {
    console.debug("document loaded");

    loadingDiv = document.getElementById("container-loading");
    inputTextarea = document.getElementById("textInput-textarea");
    renderCanvas = document.getElementById("renderTarget");
    atlasCanvas = document.getElementById("atlasCanvas");
    colorCanvas = document.getElementById("colorCanvas");
    colorInputElement = document.getElementById("textStyle-color");
    textSizeElement = document.getElementById("textStyle-size");
    localeElement = document.getElementById("textStyle-locale");
    renderClearButton = document.getElementById("renderClearButton");

    loadingDiv.style.display = "block";
    loadEmscripten();
}

function loadEmscripten() {
    console.debug("loading emscripten...");

    var script = document.createElement("script");
    script.src = "cobbletext.js";
    script.onload = function () {
        CobbletextModule().then(cobbletextLoadedCallback);
    };

    document.body.appendChild(script);
}

async function cobbletextLoadedCallback(module) {
    cobbletext = module;

    let libraryInfoElement = document.getElementById("libraryInfo");
    libraryInfoElement.textContent = "Library version: "
        + cobbletext.Library.version();

    console.debug("creating engine...");

    library = new cobbletext.Library();
    engine = new cobbletext.Engine(library);

    await loadFonts();

    loadingDiv.style.display = "none";

    attachUIEventHandlers();
}

async function loadFonts() {
    console.debug("loading fonts...");

    let pendingFonts = ["NotoSans-Regular.ttf",
        "NotoSansCJK-Regular.ttc",
        "NotoSansArabic-Regular.ttf",
        "NotoSansHebrew-Regular.ttf",
        "NotoEmoji-Regular.ttf"];

    while (pendingFonts.length) {
        let font = pendingFonts.shift();
        let fontLink = 'fonts/' + font;

        console.debug("loading font " + fontLink);

        let response = await fetch(fontLink);

        console.debug("loaded font " + response.status + " " + response.statusText);

        if (response.ok) {
            let buffer = await response.arrayBuffer();
            fontIds.push(library.loadFontBytes(buffer, 0));

            updateFontFallback();
            updateText();
        }
    }
}

function updateFontFallback() {
    if (fontIds.length < 2) {
        return;
    }

    for (let index = 0; index < fontIds.length - 1; index++) {
        library.setFontAlternative(fontIds[index], fontIds[index + 1]);
    }
}

function attachUIEventHandlers() {
    console.debug("attach ui event handlers...");

    inputTextarea.addEventListener("change", updateText);
    inputTextarea.addEventListener("input", updateText);
    colorInputElement.addEventListener("change", updateText);
    textSizeElement.addEventListener("change", updateText);
    localeElement.addEventListener("change", updateText);
    renderClearButton.addEventListener("click", function () {
        engine.clearTiles();
        updateText();
    })

    window.addEventListener("resize", updateCanvasSize);
    updateCanvasSize();
}

function updateCanvasSize() {
    renderCanvas.width = renderCanvas.offsetWidth;
    renderCanvas.height = renderCanvas.offsetHeight;
    updateText();
}

function updateText() {
    engine.clear();

    if (fontIds.length) {
        engine.font = fontIds[0];
    }

    engine.lineLength = renderCanvas.width;
    engine.fontSize = parseInt(textSizeElement.value, 10);
    engine.locale = localeElement.value;
    engine.addTextUTF8(inputTextarea.value);
    engine.layOut();

    if (!engine.tilesValid()) {
        prepareAtlas();
    }

    drawText();
}

function prepareAtlas() {
    console.debug("prepare atlas...");

    engine.rasterize();

    let atlasSize = packTiles();

    colorCanvas.width = atlasCanvas.width = atlasSize;
    colorCanvas.height = atlasCanvas.height = atlasSize;

    let context = atlasCanvas.getContext("2d");
    let tiles = engine.tiles();
    let tileCount = tiles.size();

    context.imageSmoothingEnabled = false;
    context.clearRect(0, 0, atlasCanvas.width, atlasCanvas.height);

    for (let index = 0; index < tileCount; index++) {
        let tile = tiles.get(index);
        let glyph = library.getGlyphInfo(tile.glyphID);

        atlasTable.set(glyph.id, {
            "atlasX": tile.atlasX,
            "atlasY": tile.atlasY,
            "imageWidth": glyph.imageWidth,
            "imageHeight": glyph.imageHeight,
            "imageOffsetX": glyph.imageOffsetX,
            "imageOffsetY": glyph.imageOffsetY,
        });

        drawAtlasTile(context, tile, glyph);
        glyph.image.delete();
    }

    tiles.delete();
    console.debug("drew " + tileCount + " tiles");
}

function packTiles() {
    let atlasSize = 128;

    while (true) {
        let hasOverflow = engine.packTiles(atlasSize, atlasSize);

        if (!hasOverflow) {
            break;
        }

        atlasSize *= 2;
    }

    console.debug("packed tiles to atlas size " + atlasSize);

    return atlasSize;
}

function drawAtlasTile(context, tile, glyph) {
    if (glyph.imageWidth == 0 && glyph.imageHeight == 0) {
        return;
    }

    let glyphImage = new ImageData(glyph.imageWidth, glyph.imageHeight);
    let pixelCount = glyph.imageWidth * glyph.imageHeight;

    for (let index = 0; index < pixelCount; index++) {
        glyphImage.data[index * 4 + 0] = 0xff;
        glyphImage.data[index * 4 + 1] = 0xff;
        glyphImage.data[index * 4 + 2] = 0xff;
        glyphImage.data[index * 4 + 3] = glyph.image.get(index);
    }

    context.putImageData(glyphImage, tile.atlasX, tile.atlasY);
}

function drawText() {
    let renderContext = renderCanvas.getContext("2d");
    let colorContext = colorCanvas.getContext("2d");
    let advances = engine.advances();
    let advanceCount = advances.size();
    let penX = 0;
    let penY = 0;

    renderContext.imageSmoothingEnabled = false;
    renderContext.fillStyle = "white";
    renderContext.fillRect(0, 0, renderCanvas.width, renderCanvas.height);

    colorContext.imageSmoothingEnabled = false;
    colorContext.globalCompositeOperation = "copy";

    if (colorInputElement.value.length == 7) {
        colorContext.fillStyle = colorInputElement.value + "ff";
    } else if (colorInputElement.value.length == 4) {
        colorContext.fillStyle = colorInputElement.value + "f";
    }
    colorContext.fillRect(0, 0, colorCanvas.width, colorCanvas.height);
    colorContext.globalCompositeOperation = "destination-in";
    colorContext.drawImage(atlasCanvas, 0, 0);

    for (let index = 0; index < advanceCount; index++) {
        let advance = advances.get(index);

        switch (advance.type) {
            case cobbletext.AdvanceType.Glyph:
                drawGlyph(renderContext, colorContext, advance, penX, penY);
                break;
        }

        penX += advance.advanceX;
        penY += advance.advanceY;
    }

    advances.delete();
}

function drawGlyph(context, colorContext, advance, penX, penY) {
    let atlasEntry = atlasTable.get(advance.glyphID);

    let offsetX = penX + atlasEntry.imageOffsetX + advance.glyphOffsetX;
    let offsetY = penY + atlasEntry.imageOffsetY + advance.glyphOffsetY;

    context.drawImage(colorCanvas,
        atlasEntry.atlasX, atlasEntry.atlasY,
        atlasEntry.imageWidth, atlasEntry.imageHeight,
        offsetX, offsetY,
        atlasEntry.imageWidth, atlasEntry.imageHeight
    );
}
