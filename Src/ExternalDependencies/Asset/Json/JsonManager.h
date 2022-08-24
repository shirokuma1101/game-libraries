#pragma once

#include <ExternalDependencies/Asset/IAsset/IAssetManager.h>
#include <ExternalDependencies/Asset/Json/JsonData.h>

class JsonManager : public IAssetManager<JsonData>
{
public:

    JsonManager() {}
    ~JsonManager() override { Release(); }

private:

};
