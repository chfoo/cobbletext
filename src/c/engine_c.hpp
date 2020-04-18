#pragma once

#include <memory>
#include <string>


#include "Engine.hpp"
#include "EngineImpl.hpp"
#include "c/library_c.hpp"
#include "cobbletext.h"


struct CobbletextEngine {
    CobbletextLibrary * library;
    std::unique_ptr<cobbletext::Engine> obj;
    std::vector<struct CobbletextTileInfo> tiles;
    std::vector<struct CobbletextAdvanceInfo> advances;
    std::unique_ptr<struct CobbletextTileInfo *> tilesPointer;
    std::unique_ptr<struct CobbletextAdvanceInfo *> advancesPointer;
    bool tilesPrepared;
    bool advancesPrepared;
};


namespace cobbletext::c {

    void prepareEngineTiles(struct CobbletextEngine * engine);
    void prepareEngineAdvances(struct CobbletextEngine * engine);



}
