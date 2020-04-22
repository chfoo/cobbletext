#pragma once

#include <memory>
#include <string>


#include "Engine.hpp"
#include "EngineImpl.hpp"
#include "cobbletext.h"


struct CobbletextEngine {
    CobbletextLibrary * library;
    std::unique_ptr<cobbletext::Engine> obj;
    std::unique_ptr<struct CobbletextEngineProperties> properties;
    std::unique_ptr<struct CobbletextTextProperties> textProperties;
    std::vector<std::unique_ptr<const struct CobbletextTileInfo>> tiles;
    std::vector<std::unique_ptr<const struct CobbletextAdvanceInfo>> advances;
    std::unique_ptr<const struct CobbletextTileInfo *[]> tilesPointer;
    std::unique_ptr<const struct CobbletextAdvanceInfo *[]> advancesPointer;
    std::unique_ptr<const struct CobbletextOutputInfo> outputInfo;
    bool tilesPrepared;
    bool advancesPrepared;
};


namespace cobbletext::c {

    void prepareEngineTiles(struct CobbletextEngine * engine);
    void prepareEngineAdvances(struct CobbletextEngine * engine);



}
