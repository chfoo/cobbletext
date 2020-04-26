"use strict";

var cobbletext;
var library;
var engine;
var loadingDiv;
var inputTextarea;
var renderCanvas;
var colorCanvas;
var atlasCanvas;
var atlasTable = new Map();
var colorInputElement;
var textSizeElement;

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

    loadingDiv.style.display = "block";
    loadEmscripten();
}

function loadEmscripten() {
    console.debug("loading emscripten...");

    var script = document.createElement("script");
    script.src = "cobbletext.js";
    script.onload = function () {
        cobbletext = CobbletextModule().then(cobbletextLoadedCallback);
    };

    document.body.appendChild(script);
}

function cobbletextLoadedCallback() {
    console.debug("creating engine...");

    loadingDiv.style.display = "none";

    library = new cobbletext.Library();
    engine = new cobbletext.Engine(library);

    attachUIEventHandlers();
}

function attachUIEventHandlers() {
    console.debug("attach ui event handlers...");

    inputTextarea.addEventListener("change", updateText);
    inputTextarea.addEventListener("input", updateText);
    colorInputElement.addEventListener("change", updateText);
    textSizeElement.addEventListener("change", updateText);

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
    engine.lineLength = renderCanvas.width;
    engine.fontSize = parseInt(textSizeElement.value, 10);
    engine.locale = navigator.language;
    engine.addTextUTF8(inputTextarea.value);
    engine.layOut();

    if (!engine.tilesValid()) {
        prepareAtlas();
    }

    drawText();
}

function prepareAtlas() {
    console.debug("prepare atlas...");

    let atlasSize = packTiles();

    engine.rasterize();

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
