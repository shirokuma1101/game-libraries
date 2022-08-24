#pragma once

#include <fstream>
#include <ExternalDependencies/Asset/IAsset/IAssetData.h>

#ifndef INCLUDE_NLOHMANN_JSON_HPP_
#include "Inc/nlohmann/json.hpp"
using json = nlohmann::json;
#endif // !INCLUDE_NLOHMANN_JSON_HPP_

class JsonData : public IAssetData<json>
{
public:

    JsonData(std::string_view file_path)
        : IAssetData(file_path)
    {}
    ~JsonData() override { Release(); }

    bool Load() const override {
        m_isLoaded = false;
        m_isFirstTimeLoaded = true;

        std::ifstream ifs(m_filePath);
        if (ifs) {
            ifs >> *m_upAssetData;
        }

        m_isLoaded = true;
        return static_cast<bool>(ifs);
    }

private:

};
