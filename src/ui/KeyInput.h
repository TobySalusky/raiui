//
// Created by Tobiathan on 3/20/23.
//

#pragma once

#include "Includes.h"

class KeyInput {
public:
    [[nodiscard]] static bool Down(KeyboardKey key) {
        return down.contains(key);
    }
    [[nodiscard]] static bool Pressed(KeyboardKey key) {
        return down.contains(key) && !prevDown.contains(key);
    }
    [[nodiscard]] static bool Released(KeyboardKey key) {
        return !down.contains(key) && prevDown.contains(key);
    }

    // true if left/right control or command is down - TODO: change behavior between platforms?
    [[nodiscard]] static bool SuperDown() {
        return Down(KEY_LEFT_SUPER) || Down(KEY_RIGHT_SUPER)
            || Down(KEY_LEFT_CONTROL) || Down(KEY_RIGHT_CONTROL);
    }

    // true if left/right shift is down
    [[nodiscard]] static bool ShiftDown() {
        return Down(KEY_LEFT_SHIFT) || Down(KEY_RIGHT_SHIFT);
    }

    [[nodiscard]] static const vector<char>& GetCharsPressed() {
        return charsPressed;
    }

    static void Collect();
private:
    static inline unordered_set<KeyboardKey> down = {};
    static inline unordered_set<KeyboardKey> prevDown = {};
    static inline vector<char> charsPressed = {};
};
