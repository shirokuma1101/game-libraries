#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_TEXTBOX_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_TEXTBOX_H_

#include <algorithm>
#include <ctype.h>
#include <string>

#include "Utility/Memory.h"
#include "InputManager.h"

class TextBox
{
public:

    TextBox(size_t input_max)
        : m_inputMax(input_max)
        , m_pKeyManager(new KeyManager)
    {}
    virtual ~TextBox() {
        Release();
    }

    size_t GetCursor() const noexcept {
        return m_cursor;
    }

    const std::string& GetText() const noexcept {
        return m_text;
    }

    void Update() {
        // 文字を追加 (0 ～ 9, A ～ Z)
        for (int key = '0'; key <= 'Z'; ++key) {
            // 記号はスキップ
            if (':' <= key && key <= '@') continue;
            if (m_pKeyManager->GetState(key, KeyManager::KeyState::Press)) {
                // Shiftで大文字にする
                if (m_pKeyManager->GetState(VK_SHIFT)) {
                    key = std::toupper(key);
                }
                else {
                    key = std::tolower(key);
                }
                // 文字が追加された場合、現在の文字数以下の場合はcursor+1する
                if (UpdateText(key, m_cursor) && m_cursor <= m_text.size()) {
                    ++m_cursor;
                }
                
                break;
            }
        }
        // 文字を削除
        if (m_pKeyManager->GetState(VK_BACK, KeyManager::KeyState::Press)) {
            // 文字列が消された場合、cursor-1する
            if (UpdateText('\b', m_cursor)) {
                --m_cursor;
            }
            // cursorが最後尾の場合は、そのまま最後尾にする
            if (m_cursor > m_text.size()) {
                m_cursor = m_text.size();
            }
        }
        // cursorの移動
        if (m_pKeyManager->GetState(VK_LEFT, KeyManager::KeyState::Press)) {
            if (m_cursor > 0) {
                --m_cursor;
            }
        }
        if (m_pKeyManager->GetState(VK_RIGHT, KeyManager::KeyState::Press)) {
            if (m_cursor < m_text.size()) {
                ++m_cursor;
            }
        }
        m_pKeyManager->Update();
    }

private:

    bool UpdateText(int key, size_t cursor) {
        // BSの場合
        if (key == '\b') {
            // 文字列がない場合はfalse
            if (!cursor || !m_text.size()) {
                return false;
            }
            
            // カーソルの前を消すのでcursor-1を消す
            m_text.erase(std::clamp<size_t>(cursor - 1, 0, m_text.size()), 1);
        }
        // 数字と文字の場合
        else if ('0' <= key && key <= 'z') {
            // 文字列が最大数を超える場合はfalse
            if (m_text.size() >= m_inputMax) {
                return false;
            }

            if (m_text.size() == cursor) {
                m_text.push_back(static_cast<char>(key));
            }
            else {
                std::string tmp;
                tmp.push_back(static_cast<char>(key));
                m_text.insert(cursor, tmp);
            }
        }
        return true;
    }

    void Release() {
        memory::SafeDelete(&m_pKeyManager);
    }

    KeyManager*  m_pKeyManager = nullptr;
    const size_t m_inputMax    = 0;
    
    size_t       m_cursor      = 0;
    std::string  m_text;
    
};

#endif
