//
// Created by Tobiathan on 3/11/23.
//

#include "TextInput.h"
#include "KeyBinds.h"

// helpers
bool CharIsInlineWhiteSpace(char c) {
    return c == ' ' || c == '\t';
}

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
        text = text.substr(0, my.caretPos) + inserted + text.substr(my.caretPos);
        my.caretPos += (int) inserted.length();
    }

    void TextInput::Backspace() {
        if (my.caretPos <= 0) { return; }
        text = text.substr(0, my.caretPos - 1) + text.substr(my.caretPos);
        my.caretPos--;
    }

    void TextInput::SuperBackspace() { // TODO: add mode for coders (stop on symbols too!)
        if (my.caretPos <= 0) { return; }

        char prev = text.at(my.caretPos - 1);
        Backspace();

        while (my.caretPos > 0) {
            char c = text.at(my.caretPos - 1);

            if (CharIsInlineWhiteSpace(c) && !CharIsInlineWhiteSpace(prev)) { return; }
            if (c == '\n') { return; }

            Backspace();

            prev = c;
        }
    }

    void TextInput::Delete() {
        if (my.caretPos >= text.length()) { return; }
        text = text.substr(0, my.caretPos) + text.substr(my.caretPos + 1);
    }

    void TextInput::SuperDelete() {
        if (my.caretPos >= text.length()) { return; }

        char prev = text.at(my.caretPos);
        Delete();

        while (my.caretPos < text.length()) {
            char c = text.at(my.caretPos);

            if (CharIsInlineWhiteSpace(c) && !CharIsInlineWhiteSpace(prev)) { return; }
            if (c == '\n') { return; }

            Delete();

            prev = c;
        }
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
        my.caretPos = std::max(my.caretPos - 1, 0);
    }

    void TextInput::ArrowRight() {
        my.caretPos = std::min((int) text.length(), my.caretPos + 1);
    }

    void TextInput::SuperArrowLeft() {
        if (my.caretPos <= 0) { return; }

        char prev = text.at(my.caretPos - 1);
        ArrowLeft();

        while (my.caretPos > 0) {
            char c = text.at(my.caretPos - 1);

            if (CharIsInlineWhiteSpace(c) && !CharIsInlineWhiteSpace(prev)) { return; }
            if (c == '\n') { return; }

            ArrowLeft();

            prev = c;
        }
    }

    void TextInput::SuperArrowRight() {
        if (my.caretPos >= text.length()) { return; }

        char prev = text.at(my.caretPos);
        ArrowRight();

        while (my.caretPos < text.length()) {
            char c = text.at(my.caretPos);

            if (CharIsInlineWhiteSpace(c) && !CharIsInlineWhiteSpace(prev)) { return; }
            if (c == '\n') { return; }

            ArrowRight();

            prev = c;
        }
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

    void TextInput::SetFullText(const string& newText) {
        text = newText;
        my.caretPos = (int) newText.size();
    }

} // tui