//
// Created by Tobiathan on 3/11/23.
//

#pragma once

#include "Includes.h"
#include <UIEssentials.h>
#include "KeyInput.h"
#include "Text.h"
#include "Interactive.h"

namespace tui {
    struct TextInputState {
        int caretPos = 0;
    };

    struct TextInput : StateComponent<TextInputState> {
        explicit TextInput(string& text, const style_t& style = {}, const id_t& idLike = "", tloc location = tloc::current())
                : StateComponent<TextInputState>(idLike, location), text(text)
        {
            if (IsFocused()) {
//                Log("> {} {}", KeyInput::Down(KEY_LEFT_SUPER), KeyInput::SuperDown());
                if (KeyBinds::Pressed(KeyBind::BACKSPACE) || KeyBinds::Pressed(KeyBind::SUPER_BACKSPACE)) {
//                    Log("=========== {} {}", KeyInput::Down(KEY_LEFT_SUPER), KeyInput::SuperDown());
//                    Log("it: {} {}", KeyBinds::Pressed(KeyBind::BACKSPACE), KeyBinds::Pressed(KeyBind::SUPER_BACKSPACE));
                }

                UseKeyboardInput();
            }

            // display UI
            Interactive inputContainer(combined_styles(
                    OptStyles{
                            "border:1px-black",
                            {{ IsFocused(), "border-c:orange" }}
                    },
                    style
                    ), {}, id_exact{ id });

            // TODO: split text fields from style or make them inheritable!!
            style_t textStyle = Style{
                .fontSize = style.fontSize,
                .fontStyle = style.fontStyle,
                .color = style.color
            };

            Text(text, textStyle);

            if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT) && !inputContainer.Pressed()) {
                UnFocus();
            }

            if (inputContainer.Down()) {
                Focus();
                my.caretPos = CalcCaretPos(raylib::Mouse::GetPosition());
            }
        }

        // public methods
        void SetFullText(const string& newText); // set text & move caret to end

    private:
        string& text;

        void UseKeyboardInput();

        int CalcCaretPos(Vector2 mousePos);

        bool EnactSpecialInputs();
        void TypeChar(char c);
        void InsertSequence(const string& inserted);

        void Paste();
        void Backspace();
        void SuperBackspace();
        void Delete();
        void SuperDelete();
        void ArrowLeft();
        void ArrowRight();
        void SuperArrowLeft();
        void SuperArrowRight();
    };
}
