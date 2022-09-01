#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONMANAGER_H_

#include "ExternalDependencies/Asset/IAsset/IAssetManager.h"
#include "ExternalDependencies/Asset/Json/JsonData.h"

class JsonManager : public IAssetManager<JsonData>
{
public:

    JsonManager() {}
    ~JsonManager() override { Release(); }

private:

};

#endif
