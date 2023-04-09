//
// Created by Tobiathan on 3/11/23.
//

#include "TextInput.h"
#include "KeyBinds.h"

namespace tui {
    bool TextInput::EnactSpecialInputs() {
        using enum KeyBind;




        static vector<tuple<KeyBind, std::function<void(TextInput*)>>> keyBindActions = {
                // TODO: COPY
                { PASTE, &TextInput::Paste },

                { BACKSPACE, &TextInput::Backspace },
                { SUPER_BACKSPACE, &TextInput::SuperBackspace },

                { DELETE, &TextInput::Delete },
                { SUPER_DELETE, &TextInput::SuperDelete },

                { ARROW_LEFT, &TextInput::ArrowLeft },
                { SUPER_ARROW_LEFT, &TextInput::SuperArrowLeft },

                { ARROW_RIGHT, &TextInput::ArrowRight },
                { SUPER_ARROW_RIGHT, &TextInput::SuperArrowRight },
        };

        bool specialInputOccurred = false;
        for (auto& [keyBind, action] : keyBindActions) {
            if (KeyBinds::Pressed(keyBind)) {
                action(this);
                specialInputOccurred = true;
            }
        }

        return specialInputOccurred;
    }

    void TextInput::TypeChar(char c) {
        InsertSequence(""s + c);
    }

    void TextInput::InsertSequence(const string& inserted) {
        text = text.substr(0, caretPos) + inserted + text.substr(caretPos);
        caretPos += (int) inserted.length();
    }

    void TextInput::Backspace() {
        if (caretPos <= 0) { return; }
        text = text.substr(0, caretPos - 1) + text.substr(caretPos);
        caretPos--;
    }

    void TextInput::SuperBackspace() {
        Backspace(); // TODO:
    }

    void TextInput::Delete() {
        if (caretPos >= text.length()) { return; }
        text = text.substr(0, caretPos) + text.substr(caretPos + 1);
    }

    void TextInput::SuperDelete() {
        Delete(); // TODO:
    }

    int TextInput::CalcCaretPos(Vector2 mousePos) {
        if (auto prev = UsePrev()) {
            UIElement& textElem = prev->children.front();

            int rightMostPos = 0;
            for (int i = 1; i <= text.length(); i++) {
                float charEnd = textElem.GetContentBounds().x +
                        (float) raylib::Text(
                                Fonts::Get(textElem.fontStyle),
                                text.substr(0, i),
                                textElem.fontSize
                        ).Measure();

                if (mousePos.x >= charEnd) {
                    rightMostPos = i;
                }
            }
            return rightMostPos;
        }
        return 0;
    }

    void TextInput::ArrowLeft() {
        caretPos = std::max(caretPos - 1, 0);
    }

    void TextInput::ArrowRight() {
        caretPos = std::min((int) text.length(), caretPos + 1);
    }

    void TextInput::SuperArrowLeft() {
        ArrowLeft(); // TODO:
    }

    void TextInput::SuperArrowRight() {
        ArrowRight(); // TODO:
    }

    void TextInput::Paste() {
        InsertSequence(GetClipboardText());
    }

    void TextInput::UseKeyboardInput() {
        bool specialEventOccurred = EnactSpecialInputs();

        if (specialEventOccurred) { return; }

        for (char c : KeyInput::GetCharsPressed()) {
            TypeChar(c);
        }
    }

} // tui