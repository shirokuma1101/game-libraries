#pragma once

#include <vector>
#include <list>
#include <map>

#include "SimpleUniqueThread.h"

class SimpleThreadManager {
public:

    using ID = std::thread::id;

    SimpleThreadManager() noexcept {
        m_upThreads.clear();
    }

    ~SimpleThreadManager() noexcept {
        for (auto&& e : m_upThreads) {
            e.second->SyncEnd();
        }
        m_upThreads.clear();
    }

    template<class Func, class Inst, class...Args>
    ID Create(Func func, Inst inst, Args... args) {
        std::unique_ptr<SimpleUniqueThread> upThr = std::make_unique<SimpleUniqueThread>();
        upThr->Create(func, inst, args...);
        return m_upThreads.emplace(upThr->GetID(), std::move(upThr)).first;
    }

    bool IsEnd(ID id) const noexcept {
        if (auto iter = m_upThreads.find(id); iter != m_upThreads.end()) {
            return iter->second->IsEnd();
        }
        else {
            assert::RaiseAssert("thread is not exists");
            return false;
        }
    }

    void SyncEnd(ID* id, SimpleUniqueThread::SyncType sync_type = SimpleUniqueThread::SyncType::JOIN) {
        if (auto iter = m_upThreads.find(*id); iter != m_upThreads.end()) {
            iter->second->SyncEnd(sync_type);
            m_upThreads.erase(*id);
            *id = ID();
        }
        else {
            assert::RaiseAssert("thread is not exists");
        }
    }

    void AutoEnd() {
        for (auto iter = m_autoSyncEndIDs.begin(); iter != m_autoSyncEndIDs.end();) {
            if (m_upThreads.count(*iter)) {
                if (IsEnd(*iter)) {
                    SyncEnd(&(*iter));
                    iter = m_autoSyncEndIDs.erase(iter);
                }
                else {
                    ++iter;
                }
            }
        }
    }

private:

    std::map<ID, std::unique_ptr<SimpleUniqueThread>> m_upThreads;
    std::vector<ID>                                   m_autoSyncEndIDs;

};
