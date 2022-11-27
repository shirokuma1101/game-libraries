#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONDATA_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONDATA_H_

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

#include "ExternalDependencies/Asset/IAsset/IAssetData.h"

#include "nlohmann/json.hpp"
#include "nlohmann/json-schema.hpp"
#pragma comment(lib, "nlohmann_json_schema_validator.lib")


/**************************************************
*
* A class that implements the asset interface of
* json(nlohmann_json) with
* validator(nlohmann_json_schema_validator)
*
**************************************************/
class JsonData : public IAssetData<nlohmann::json>
{
public:

    using Json          = nlohmann::json;
    using JsonValidator = nlohmann::json_schema::json_validator;

    JsonData(std::string_view file_path, std::shared_ptr<std::unordered_map<std::string, JsonData::JsonValidator>> validators = nullptr)
        : IAssetData(file_path)
        , m_wpValidators(validators)
    {}
    virtual ~JsonData() override {
        Release();
    }

    bool Load() override {
        return LoadProcess([&] {
            Json json;
            std::ifstream ifs(m_filePath);
            if (!ifs) return false;
            ifs >> json;
            if (ValidateJson(json)) {
                *m_upAssetData = json;
                return true;
            }
            return false;
        });
    }

private:
    
    bool ValidateJson(const Json& data) const {
        if (m_wpValidators.expired() || !data.count("schema") || !data.at("schema").is_string()) {
            return true;
        }
        std::string schema_name = data.at("schema").get<std::string>();
        if (m_wpValidators.lock()->count(schema_name.data())) {
            try {
                m_wpValidators.lock()->at(schema_name.data()).validate(data);
                return true;
            }
            catch (const std::exception& e) {
                assert::ShowError(ASSERT_FILE_LINE, "Validation of json failed: " + m_filePath + " - " + std::string(e.what()));
                return false;
            }
        }
        else {
            assert::ShowError(ASSERT_FILE_LINE, "Schema name not found: " + m_filePath + " - " + schema_name);
            return false;
        }
    }

    std::weak_ptr<std::unordered_map<std::string, JsonData::JsonValidator>> m_wpValidators;

};

#endif
