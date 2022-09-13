﻿#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_IASSET_IASSETDATA_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_IASSET_IASSETDATA_H_

#include <memory>
#include <string>
#include <string_view>

#include "Thread/SimpleThreadManager/SimpleUniqueThread.h"

template<class _AssetClass>
class IAssetData
{
public:

    using AssetClass = typename _AssetClass;

    IAssetData(std::string_view path)
        : m_isLoaded(false)
        , m_isFirstTimeLoaded(false)
        , m_isLoadedOnlyOnce(false)
        , m_filePath(path)
        , m_upAssetData(std::make_unique<AssetClass>())
    {}
    virtual ~IAssetData() { Release(); }

    virtual bool Load() const = 0;

    virtual void AsyncLoad(bool force = false) final {
        if (!m_thread.IsEnd()) return;
        if ((!m_thread.IsExists() && !m_isFirstTimeLoaded) || force) {
            m_thread.CreateAuto(&IAssetData::Load, this);
        }
    }

    virtual bool IsLoaded() const final {
        return m_thread.IsExists() ? m_thread.IsEnd() : m_isLoaded;
    }

    virtual bool IsLoadSuccessed() const final {
        return m_isLoadSuccessed;
    }

    virtual bool IsLoadedOnlyOnce() const final {
        if (IsLoaded() && !m_isLoadedOnlyOnce) {
            m_isLoadedOnlyOnce = true;
            return true;
        }
        else return false;
    }

    virtual void LoadedOnlyOnceReset() final {
        m_isLoadedOnlyOnce = false;
    }

    virtual std::string GetFilePath() const final {
        return m_filePath;
    }

    const auto& GetData() const {
        return m_upAssetData;
    }

protected:

    template<class Func>
    bool LoadProcess(Func&& func) const {
        m_isLoaded = false;
        m_isFirstTimeLoaded = true;

        m_isLoadSuccessed = func();

        m_isLoaded = true;

        return m_isLoadSuccessed;
    }

    void Release() {
        if (m_thread.IsExists()) {
            while (true) {
                if (m_thread.IsEnd()) break;
            }
        }
    }

    SimpleUniqueThread                m_thread;
    mutable bool                      m_isLoaded          = false;
    mutable bool                      m_isLoadSuccessed   = false;
    mutable bool                      m_isFirstTimeLoaded = false;
    mutable bool                      m_isLoadedOnlyOnce  = false;

    const std::string                 m_filePath;
    const std::unique_ptr<AssetClass> m_upAssetData       = nullptr;

private:

};

#endif
