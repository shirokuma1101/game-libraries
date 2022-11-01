#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_IASSET_IASSETMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_IASSET_IASSETMANAGER_H_

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "Utility/Assert.h"
#include "ExternalDependencies/Asset/Json/JsonData.h"

#pragma warning(push)
#pragma warning(disable:4172) // returning address of local variable or temporary


/**************************************************
*
* Manager interface of AssetData implementation class
*
**************************************************/
template<class AssetDataImpl>
class IAssetManager
{
public:

    using AssetDataImplT = typename AssetDataImpl;

    IAssetManager() {}
    virtual ~IAssetManager() {
        Release();
    }

    virtual void Register(std::string_view file_path) {
        auto json_data = std::make_unique<JsonData>(file_path);
        if (!json_data->Load()) {
            assert::ShowError(ASSERT_FILE_LINE, "Path not found: " + std::string(file_path.data()));
            return;
        }
        for (const auto& e : json_data->GetData()->at("list")) {
            auto name = e.at("name").get<std::string>();
            auto path = e.at("path").get<std::string>();
            auto asset_data = std::make_unique<AssetDataImpl>(path);
            m_upAssets.emplace(name, std::move(asset_data));
        }
    }
    virtual void Register(const std::unordered_map<std::string, std::unique_ptr<JsonData>>& jsons, std::initializer_list<std::string_view> keys = {}) {
        for (const auto& e : jsons) {
            auto& json_data = e.second;
            if (!json_data->IsLoaded()) {
                assert::ShowWarning(ASSERT_FILE_LINE, "json data not loaded. Load (loading may take some time.)");
                if (!json_data->Load()) {
                    assert::ShowError(ASSERT_FILE_LINE, "Path not found: " + std::string(json_data->GetFilePath()));
                    break;
                }
            }

            if (!e.second->IsLoadSuccessed()) continue;
            auto copy_data = *e.second->GetData();
            if (keys.size()) {
                for (const auto& key : keys) {
                    if (copy_data.count(key)) {
                        copy_data = copy_data.at(key.data());
                    }
                    else return;
                }
            }
            
            auto& name = e.first;
            auto path = copy_data.at("path").get<std::string>();
            auto asset_data = std::make_unique<AssetDataImpl>(path);
            m_upAssets.emplace(name, std::move(asset_data));
        }
    }

    virtual const std::unordered_map<std::string, std::unique_ptr<AssetDataImpl>>& GetAssets() const final {
        return m_upAssets;
    }
    virtual const std::unique_ptr<AssetDataImpl>& GetAsset(std::string_view name) const final {
        if (auto iter = m_upAssets.find(name.data()); iter != m_upAssets.end()) {
            return iter->second;
        }
        assert::ShowError(ASSERT_FILE_LINE, "Asset not found: " + std::string(name.data()));
        return nullptr;
    }
    virtual const typename AssetDataImpl::AssetClassT& operator[](std::string_view name) const final {
        return *GetAsset(name)->GetData();
    }

    virtual void Load() final {
        for (const auto& e : m_upAssets) {
            if (!e.second->Load()) {
                assert::ShowError(ASSERT_FILE_LINE, "Asset load failed: " + e.first);
            }
        }
    }
    virtual bool Load(std::string_view name) const final {
        if (auto& asset = GetAsset(name); asset) {
            if (asset->Load()) {
                return true;
            }
            assert::ShowError(ASSERT_FILE_LINE, "Path not found: " + std::string(GetFilePath(name).data()));
        }
        return false;
    }
    virtual bool AsyncLoad(std::string_view name, bool force = false) const final {
        if (auto& asset = GetAsset(name); asset) {
            asset->AsyncLoad(force);
            return true;
        }
        return false;
    }

    virtual bool IsLoaded(std::string_view name) const final {
        if (auto& asset = GetAsset(name); asset) {
            return asset->IsLoaded();
        }
        return false;
    }
    virtual bool IsLoadedOnlyOnce(std::string_view name) const final {
        if (auto& asset = GetAsset(name); asset) {
            return asset->IsLoadedOnlyOnce();
        }
        return false;
    }
    virtual bool LoadedOnlyOnceReset(std::string_view name) const final {
        if (auto& asset = GetAsset(name); asset) {
            asset->LoadedOnlyOnceReset();
            return true;
        }
        return false;
    }

    virtual const std::string GetFilePath(std::string_view name) const final {
        if (auto& asset = GetAsset(name); asset) {
            return asset->GetFilePath();
        }
        return std::string();
    }

    virtual const std::unique_ptr<typename AssetDataImpl::AssetClassT>& GetData(std::string_view name) const final {
        return GetAsset(name)->GetData();
    }

    std::shared_ptr<typename AssetDataImpl::AssetClassT> CopyData(std::string_view name) {
        return std::make_shared<typename AssetDataImpl::AssetClassT>(*GetData(name).get());
    }

    virtual void Release() noexcept {
        m_upAssets.clear();
    }

protected:

    std::unordered_map<std::string, std::unique_ptr<AssetDataImpl>> m_upAssets;

};

#pragma warning(pop)

#endif
