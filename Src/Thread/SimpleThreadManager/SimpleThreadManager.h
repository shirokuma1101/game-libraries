#pragma once

#ifndef GAME_LIBRARIES_THREAD_SIMPLETHREADMANAGER_SIMPLETHREADMANAGER_H_
#define GAME_LIBRARIES_THREAD_SIMPLETHREADMANAGER_SIMPLETHREADMANAGER_H_

#include <map>
#include <memory>
#include <vector>

#include "SimpleUniqueThread.h"

class SimpleThreadManager {
public:

    using ID = std::thread::id;

    SimpleThreadManager() noexcept {
        m_upThreads.clear();
    }
    virtual ~SimpleThreadManager() noexcept {
        Release();
    }

    template<class Func, class Inst, class...Args>
    ID Create(Func func, Inst inst, Args... args) const {
        auto thread = std::make_unique<SimpleUniqueThread>();
        thread->Create(func, inst, args...);
        return m_upThreads.emplace(thread->GetID(), std::move(thread)).first;
    }

    bool IsEnd(ID id) const noexcept {
        if (auto iter = m_upThreads.find(id); iter != m_upThreads.end()) {
            return iter->second->IsEnd();
        }
        assert::RaiseAssert(ASSERT_FILE_LINE, "thread is not exists");
        return false;
    }

    void SyncEnd(ID* id, SimpleUniqueThread::SyncType sync_type = SimpleUniqueThread::SyncType::JOIN) {
        if (auto iter = m_upThreads.find(*id); iter != m_upThreads.end()) {
            iter->second->SyncEnd(sync_type);
            m_upThreads.erase(*id);
            *id = ID();
            return;
        }
        assert::RaiseAssert(ASSERT_FILE_LINE, "thread is not exists");
    }

private:

    void Release() noexcept {
        for (auto&& e : m_upThreads) {
            e.second->SyncEnd();
        }
        m_upThreads.clear();
    }

    std::map<ID, std::unique_ptr<SimpleUniqueThread>> m_upThreads;

};

#endif
