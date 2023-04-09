//
// Created by Tobiathan on 3/22/23.
//

#pragma once

#include <stdexcept>
#include "KeyInput.h"

enum class KeyBind {
    BACKSPACE, SUPER_BACKSPACE,
    DELETE, SUPER_DELETE,
    COPY, PASTE,
    ARROW_LEFT, SUPER_ARROW_LEFT,
    ARROW_RIGHT, SUPER_ARROW_RIGHT,
    ARROW_UP, SUPER_ARROW_UP,
    ARROW_DOWN, SUPER_ARROW_DOWN,
};

class KeyBinds {
public:
    static bool Pressed(KeyBind keyBind) {
        using enum KeyBind;

        switch (keyBind) {
            case BACKSPACE:
                return !KeyInput::SuperDown() && KeyInput::Pressed(KEY_BACKSPACE);
            case SUPER_BACKSPACE:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_BACKSPACE);

            case DELETE:
                return !KeyInput::SuperDown() && KeyInput::Pressed(KEY_DELETE);
            case SUPER_DELETE:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_DELETE);

            case COPY:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_C);
            case PASTE:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_V);

            case ARROW_LEFT:
                return !KeyInput::SuperDown() && KeyInput::Pressed(KEY_LEFT);
            case SUPER_ARROW_LEFT:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_LEFT);

            case ARROW_RIGHT:
                return !KeyInput::SuperDown() && KeyInput::Pressed(KEY_RIGHT);
            case SUPER_ARROW_RIGHT:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_RIGHT);

            case ARROW_UP:
                return !KeyInput::SuperDown() && KeyInput::Pressed(KEY_UP);
            case SUPER_ARROW_UP:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_UP);

            case ARROW_DOWN:
                return !KeyInput::SuperDown() && KeyInput::Pressed(KEY_DOWN);
            case SUPER_ARROW_DOWN:
                return KeyInput::SuperDown() && KeyInput::Pressed(KEY_DOWN);

            default:
                throw std::runtime_error("No such keyBind!");
        }
    }
};
