#pragma once

#ifndef GAME_LIBRARIES_THREAD_SIMPLETHREADMANAGER_SIMPLEUNIQUETHREAD_H_
#define GAME_LIBRARIES_THREAD_SIMPLETHREADMANAGER_SIMPLEUNIQUETHREAD_H_

#include <memory>
#include <thread>

#include "Utility/Assert.h"

//TODO future, promise

class SimpleUniqueThread {
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

    ~SimpleUniqueThread() noexcept {
        Release();
    }

    template<class Func, class Inst, class... Args>
    void Create(Func func, Inst inst, Args... args) {
        CheckNoExists();
        m_isEnd = false;
        m_upThread = std::make_unique<std::thread>(
            &SimpleUniqueThread::Run<Func, Inst, Args...>,
            this,
            &m_isEnd, func, inst, args...
        );
    }

    template<class Func, class Inst, class... Args>
    void CreateAuto(Func func, Inst inst, Args... args) {
        CheckNoExists();
        m_isEnd = false;
        m_upThread = std::make_unique<std::thread>(
            &SimpleUniqueThreadEx::AutoRun<Func, Inst, Args...>,
            this,
            &m_isEnd, func, inst, args...
        );
    }

    bool IsEnd() const noexcept {
        return m_isEnd;
    }

    bool IsExists() const noexcept {
        return static_cast<bool>(m_upThread);
    }

    std::thread::id GetID() const noexcept {
        CheckExists();
        return m_upThread->get_id();
    }

    void SyncEnd(SyncType sync_type = SyncType::JOIN) {
        CheckExists();
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
        m_upThread = nullptr;
    }

private:

    template<class Func, class Inst, class... Args>
    void Run(bool* is_end, Func func, Inst inst, Args... args) {
        (inst->*func)(args...);
        *is_end = true;
    }

    template<class Func, class Inst, class... Args>
    void AutoRun(bool* is_end, Func func, Inst inst, Args... args) {
        (inst->*func)(args...);
        SyncEnd(SyncType::DETACH);
        *is_end = true;
    }

    void CheckExists() const noexcept {
        if (m_upThread) return;
        assert::RaiseAssert("thread is not exists");
    }
    void CheckNoExists() const noexcept {
        if (!m_upThread) return;
        assert::RaiseAssert("thread is exists");
    }

    // make std::function
    template<class Func, class Inst, class... Args>
    auto ToFuncObj(const Func& func, const Inst& inst, const Args&... args) {
        std::function<decltype((inst->func)(args...))(typename std::decay_t<const Args>...)> std_func
            = [&](const decltype(args)&... args) {
            return (inst->func)(args...);
        };
        return std_func(args...);
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
