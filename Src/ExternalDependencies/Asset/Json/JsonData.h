#pragma once

#include <fstream>

#include "ExternalDependencies/Asset/IAsset/IAssetData.h"

#ifndef INCLUDE_NLOHMANN_JSON_HPP_
#include "nlohmann/json.hpp"
#endif // !INCLUDE_NLOHMANN_JSON_HPP_

class JsonData : public IAssetData<nlohmann::json>
{
public:

    using json = nlohmann::json;

    JsonData(std::string_view file_path)
        : IAssetData(file_path)
    {}
    ~JsonData() override { Release(); }

    bool Load() const override {
        return LoadProcess([&] {
            std::ifstream ifs(m_filePath);
            if (ifs) {
                ifs >> *m_upAssetData;
            }
            return static_cast<bool>(ifs);
        });
    }

private:

};
