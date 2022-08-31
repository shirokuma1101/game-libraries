#pragma once

#include <list>
#include <map>
#include <memory>
#include <vector>

#include "SimpleUniqueThreadEx.h"

class SimpleThreadManagerEx {
public:

    using ID = std::thread::id;

    SimpleThreadManagerEx() noexcept {
        m_upThreads.clear();
    }

    ~SimpleThreadManagerEx() noexcept {
        for (auto&& e : m_upThreads) {
            e.second->SyncEnd();
        }
        m_upThreads.clear();
    }

    template<class Func, class Inst, class...Args>
    ID Create(Func func, Inst inst, Args... args) const {
        std::unique_ptr<SimpleUniqueThread> th = std::make_unique<SimpleUniqueThread>();
        th->Create(func, inst, args...);
        return m_upThreads.emplace(th->GetID(), std::move(th)).first;
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

    void SyncEnd(ID* id, SimpleUniqueThreadEx::SyncType sync_type = SimpleUniqueThreadEx::SyncType::JOIN) {
        if (auto iter = m_upThreads.find(*id); iter != m_upThreads.end()) {
            iter->second->SyncEnd(sync_type);
            m_upThreads.erase(*id);
            *id = ID();
        }
        else {
            assert::RaiseAssert("thread is not exists");
        }
    }

private:

    std::map<ID, std::unique_ptr<SimpleUniqueThreadEx>> m_upThreads;
    std::vector<ID>                                     m_autoSyncEndIDs;

};
