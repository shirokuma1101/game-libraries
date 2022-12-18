#pragma once

#include "nlohmann/json.hpp"

struct IJsonData {
    virtual void FromJson(const nlohmann::json& json) = 0;
};

template <class T>
class JsonHolder
{
public:

    JsonHolder(nlohmann::json* json)
        : m_pJson(json)
    {
        static_assert(std::is_base_of_v<IJsonData, T>, "T must be derived from IJsonData");
        Load();
    }

    T& Get() {
        return m_data;
    }
    const T& Get() const {
        return m_data;
    }

    bool Load() {
        if (!m_pJson) return false;
        m_data.FromJson(*m_pJson);
        return true;
    }

private:

    const nlohmann::json* m_pJson = nullptr;
    
    T                    m_data;

};
