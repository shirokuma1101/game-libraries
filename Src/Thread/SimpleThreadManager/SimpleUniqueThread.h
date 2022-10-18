#pragma once

#ifndef GAME_LIBRARIES_THREAD_SIMPLETHREADMANAGER_SIMPLEUNIQUETHREAD_H_
#define GAME_LIBRARIES_THREAD_SIMPLETHREADMANAGER_SIMPLEUNIQUETHREAD_H_

#include <memory>
#include <thread>

#include "Utility/Assert.h"

//TODO future, promise

class SimpleUniqueThread
{
public:

    enum class SyncType {
        JOIN,
        DETACH,
        TERMINATE,
    };

    SimpleUniqueThread() noexcept
        : m_isEnd(true)
        , m_upThread(nullptr)
    {}
    virtual ~SimpleUniqueThread() noexcept {
        Release();
    }

    bool IsEnd(bool enable_assert = false) const noexcept {
        if (m_isEnd) {
            return true;
        }
        if (enable_assert) {
            assert::RaiseAssert(ASSERT_FILE_LINE, "thread is not end");
        }
        return false;
    }
    bool IsExists(bool enable_assert = false) const noexcept {
        if (m_upThread) {
            return true;
        }
        if (enable_assert) {
            assert::RaiseAssert(ASSERT_FILE_LINE, "thread is not exists");
        }
        return false;
    }
    bool IsNoExists(bool enable_assert = false) const noexcept {
        if (!m_upThread) {
            return true;
        }
        if (enable_assert) {
            assert::RaiseAssert(ASSERT_FILE_LINE, "thread is exists");
        }
        return false;
    }

    template<class Func, class Inst, class... Args>
    void Create(Func func, Inst inst, Args... args) {
        if (IsNoExists(true)) {
            m_isEnd = false;
            m_upThread = std::make_unique<std::thread>(
                &SimpleUniqueThread::Run<Func, Inst, Args...>,
                this,
                &m_isEnd, func, inst, args...
            );
        }
    }
    template<class Func, class Inst, class... Args>
    void CreateAutoEnd(Func func, Inst inst, Args... args) {
        if (IsNoExists(true)) {
            m_isEnd = false;
            m_upThread = std::make_unique<std::thread>(
                &SimpleUniqueThread::RunAutoEnd<Func, Inst, Args...>,
                this,
                &m_isEnd, func, inst, args...
            );
        }
    }

    std::thread::id GetID() const noexcept {
        if (IsExists(true)) {
            return m_upThread->get_id();
        }
        return std::thread::id();
    }

    void SyncEnd(SyncType sync_type = SyncType::JOIN) {
        if (IsExists(true)) {
            switch (sync_type) {
            case SyncType::JOIN:
                m_upThread->join();
                break;
            case SyncType::DETACH:
                m_upThread->detach();
                break;
            case SyncType::TERMINATE:
                m_upThread->~thread();
                break;
            }
        }
        m_upThread = nullptr;
    }

private:

    template<class Func, class Inst, class... Args>
    void Run(bool* is_end, Func func, Inst inst, Args... args) {
        (inst->*func)(args...);
        *is_end = true;
    }
    template<class Func, class Inst, class... Args>
    void RunAutoEnd(bool* is_end, Func func, Inst inst, Args... args) {
        (inst->*func)(args...);
        SyncEnd(SyncType::DETACH);
        *is_end = true;
    }
    
    void Release() noexcept {
        if (IsExists()) {
            SyncEnd();
        }
    }

    bool                         m_isEnd    = false;
    std::unique_ptr<std::thread> m_upThread = nullptr;

};

#endif
