#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <Utility/Assert.h>
#include <ExternalDependencies/Asset/Json/JsonData.h>
#include "IAssetData.h"

#pragma warning(push)
#pragma warning(disable:4172) // returning address of local variable or temporary

template<class AssetDataImpl>
class IAssetManager
{
public:

    IAssetManager() {}
    virtual ~IAssetManager() { Release(); }

    virtual void Register(std::string_view file_path) final {
        m_upJsonData = std::make_unique<JsonData>(file_path);
        if (!m_upJsonData->Load()) {
            assert::RaiseAssert(std::string("Path not found: ") + file_path.data());
        }

        for (const auto& e : *m_upJsonData->GetData()) {
            auto name = e.at("name").get<std::string>();
            auto path = e.at("path").get<std::string>();
            auto asset_data = std::make_unique<AssetDataImpl>(path);
            m_upAssets.emplace(name, std::move(asset_data));
        }
    }

    virtual const std::unique_ptr<AssetDataImpl>& GetAsset(std::string_view name) const final {
        if (auto iter = m_upAssets.find(name.data()); iter != m_upAssets.end()) {
            return iter->second;
        }
        assert::RaiseAssert(std::string("Asset not found: ") + name.data());
        return nullptr;
    }

    //virtual const std::unique_ptr<AssetDataImpl>& operator[] (std::string_view name) const final {
    //    return GetAsset(name);
    //}
    virtual const typename AssetDataImpl::AssetClass& operator[] (std::string_view name) const final {
        return *GetAsset(name)->GetData();
    }

    virtual void Load() final {
        for (const auto& e : m_upAssets) {
            e.second->Load();
        }
    }

    virtual void Load(std::string_view name) final {
        if (!GetAsset(name)->Load()) {
            assert::RaiseAssert(std::string("Path not found: ") + GetFilePath(name).data());
        }
    }
    
    virtual void AsyncLoad(std::string_view name, bool force = false) final {
        return GetAsset(name)->AsyncLoad(force);
    }

    virtual bool IsLoaded(std::string_view name) const final {
        return GetAsset(name)->IsLoaded();
    }

    virtual bool IsLoadedOnlyOnce(std::string_view name) const final {
        return GetAsset(name)->IsLoadedOnlyOnce();
    }

    virtual void LoadedOnlyOnceReset(std::string_view name) final {
        GetAsset(name)->LoadedOnlyOnceReset();
    }

    virtual const std::string GetFilePath(std::string_view name) const final {
        return GetAsset(name)->GetFilePath();
    }

    virtual const std::unique_ptr<typename AssetDataImpl::AssetClass>& GetData(std::string_view name) const final {
        return GetAsset(name)->GetData();
    }

    std::shared_ptr<typename AssetDataImpl::AssetClass> CopyData(std::string_view name) {
        return std::make_shared<typename AssetDataImpl::AssetClass>(*GetData(name).get());
    }

    virtual void Release() noexcept {
        m_upJsonData.reset();
        m_upAssets.clear();
    }

protected:

    std::unique_ptr<JsonData>                                       m_upJsonData;
    std::unordered_map<std::string, std::unique_ptr<AssetDataImpl>> m_upAssets;

private:

};

#pragma warning(pop)
