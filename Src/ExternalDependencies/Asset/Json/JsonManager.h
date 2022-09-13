#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONMANAGER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONMANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "ExternalDependencies/Asset/IAsset/IAssetManager.h"
#include "ExternalDependencies/Asset/Json/JsonData.h"

class JsonManager : public IAssetManager<JsonData>
{
public:

    JsonManager()
        : m_spValidators(std::make_shared<std::unordered_map<std::string, JsonData::JsonValidator>>())
    {}
    ~JsonManager() override { Release(); }

    virtual void Register(std::string_view file_path) override {
        auto json_data = std::make_unique<JsonData>(file_path);
        if (!json_data->Load()) {
            assert::RaiseAssert(std::string("Path not found: ") + file_path.data());
            return;
        }
        for (const auto& e : json_data->GetData()->at("schema")) {
            auto name = e.at("name").get<std::string>();
            auto path = e.at("path").get<std::string>();
            JsonData schema(path);
            if (!schema.Load()) {
                assert::RaiseAssert("Path not found: " + std::string(path));
                return;
            }
            JsonData::JsonValidator validator;
            try {
                validator.set_root_schema(*schema.GetData());
            }
            catch (const std::exception& e) {
                assert::RaiseAssert("Validation of schema failed: " + std::string(e.what()));
                return;
            }
            m_spValidators->emplace(name, std::move(validator));
        }
        for (const auto& e : json_data->GetData()->at("list")) {
            auto name = e.at("name").get<std::string>();
            auto path = e.at("path").get<std::string>();
            auto asset_data = std::make_unique<JsonData>(path, m_spValidators);
            m_upAssets.emplace(name, std::move(asset_data));
        }
    }

    using IAssetManager<JsonData>::Register;

private:

    std::shared_ptr<std::unordered_map<std::string, JsonData::JsonValidator>> m_spValidators;

};

#endif
