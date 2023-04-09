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
    struct TextInput : UIComponent {
        explicit TextInput(string& text, const style_t& style = {}, const id_t& idLike = "", tloc location = tloc::current())
                : UIComponent(idLike, location), text(text),
                    caretPos(UseRef<int>((int) text.length())),
                    active(UseRef(false))
        {

            if (active) {
                UseKeyboardInput();
            }

            // display UI
            Interactive inputContainer(OptStyles{
                    "border:1px-black",
                    {{ active, "border-c:orange" }}
                }, {}, id_exact{ id });
            Text(text, style);

            if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT) && !inputContainer.Pressed()) {
                active = false;
            }

            if (inputContainer.Down()) {
                active = true;
                caretPos = CalcCaretPos(raylib::Mouse::GetPosition());
            }
        }

    private:
        string& text;
        int& caretPos;
        bool& active;

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
