#pragma once

#include <thread>

#include "..\..\Utility\assert.h"

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

    //template<class Func, class Inst, class T*, class... Args>
    //void Create(Func func, Inst inst, T* ret, Args... args) {
    //    CheckNoExists();
    //    m_isEnd = false;
    //    m_upThread = std::make_unique<std::thread>(
    //        &SimpleUniqueThread::Run<Func, Inst, T*, Args...>,
    //        this,
    //        &m_isEnd, func, inst, ret, args...
    //        );
    //}

    bool IsEnd() const noexcept {
        return m_isEnd;
    }

    std::thread::id GetID() const noexcept{
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

    template<class Func, class Inst, class T*, class... Args>
    void Run(bool* is_end, Func func, Inst inst, T* ret, Args... args) {
        *ret = (inst->*func)(args...);
        *is_end = true;
    }

    void CheckExists() const noexcept {
        if (m_upThread) return;
        assert::RaiseAssert("ë∂ç›ÇµÇƒÇ¢Ç‹ÇπÇÒÅB");
    }
    void CheckNoExists() const noexcept {
        if (!m_upThread) return;
        assert::RaiseAssert("ë∂ç›ÇµÇƒÇ¢Ç‹Ç∑ÅB");
    }

    void Release() noexcept {
        SyncEnd();
    }

    bool                         m_isEnd    = false;
    std::unique_ptr<std::thread> m_upThread = nullptr;

};
