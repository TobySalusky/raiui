//
// Created by Tobiathan on 2/27/23.
//

#pragma once

#include <UIEssentials.h>

namespace tui {
    struct InteractiveStyles {
        style_t hover = {};
        style_t active = {};
    };


    struct InteractiveClass {
        string className;
        [[nodiscard]] InteractiveStyles CreateInteractiveStyles() const {
            return { .hover = className + ":hover", .active = className + ":active" };
        }
    };

    struct Interactive : public OpenUIComponent {
        explicit Interactive(const InteractiveClass& interactiveClass, const id_t& idLike = "", tloc location = tloc::current())
                : Interactive(interactiveClass.className, interactiveClass.CreateInteractiveStyles(), idLike, location) {}


            explicit Interactive(const style_t& style = {}, const InteractiveStyles& interactiveStyles = {}, const id_t& idLike = "", tloc location = tloc::current())
                : OpenUIComponent(idLike, location) {
            bool& held = UseRef(false, id_append { id });
            optional<float>& lastPressed = UseRef<optional<float>>(std::nullopt, id_append { id });

            if (auto prevRes = UsePrev()) {
                hovered = prevRes->GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

                if (hovered && raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
                    pressed = true;
                    held = true;

                    // double press handling
                    if (lastPressed && GetTime() - *lastPressed < DOUBLE_PRESS_THRESHOLD) {
                        doublePressed = true;
                        lastPressed = std::nullopt;
                    } else {
                        lastPressed = { GetTime() };
                    }
                }

                if (raylib::Mouse::IsButtonReleased(MOUSE_BUTTON_LEFT) || !raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
                    released = true;
                    held = false;
                }

                down = held;
            }

            style_t interactiveStyle = OptStyles {
                style,{
                    {down, interactiveStyles.active},
                    {hovered, interactiveStyles.hover},
                }};

            DOM::Current().Attach({.id = id, .style = interactiveStyle});
        }

        [[nodiscard]] bool Pressed() const { return pressed; }
        [[nodiscard]] bool Released() const { return released; }

        [[nodiscard]] bool Down() const { return down; }
        [[nodiscard]] bool Hovered() const { return hovered; }

        [[nodiscard]] bool DoublePressed() const { return doublePressed; }

    private:
        bool pressed = false;
        bool doublePressed = false;
        bool released = false;
        bool down = false;
        bool hovered = false;

        constexpr static float DOUBLE_PRESS_THRESHOLD = 0.5f;
    };
}
