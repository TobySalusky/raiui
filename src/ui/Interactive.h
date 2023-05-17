//
// Created by Tobiathan on 2/27/23.
//

#pragma once

#include <UIEssentials.h>
#include "KeyBinds.h"

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

    struct InteractiveState {
        bool held = false;
        optional<float> lastPressed = std::nullopt;
    };

    struct Interactive : public OpenStateComponent<InteractiveState> {
        explicit Interactive(const InteractiveClass& interactiveClass, const id_t& idLike = "", tloc location = tloc::current())
                : Interactive(interactiveClass.className, interactiveClass.CreateInteractiveStyles(), idLike, location) {}

        explicit Interactive(const style_t& style = {}, const InteractiveStyles& interactiveStyles = {}, const id_t& idLike = "", tloc location = tloc::current())
            : OpenStateComponent<InteractiveState>(idLike, location) {

            if (auto prevRes = UsePrev()) {
                hovered = prevRes->GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

                // key input
                if (KeyBinds::Pressed(KeyBind::INPUT_ENTER) && IsFocused()) {
                    OnClick();
                }

                // mouse inputs
                if (hovered && raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
                    my.held = true;
                    OnClick();
                }

                if (raylib::Mouse::IsButtonReleased(MOUSE_BUTTON_LEFT) || !raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
                    released = true;
                    my.held = false;
                }
            }

            style_t interactiveStyle = OptStyles {
                    combined_styles(Style{ .tabIndex = 0 }, style),
                    {
                        {Down(), interactiveStyles.active},
                        {Hovered(), interactiveStyles.hover},
                    }
            };

            DOM::Current().Attach({.id = id, .style = interactiveStyle});
        }

        [[nodiscard]] bool Pressed() const { return pressed; }
        [[nodiscard]] bool Released() const { return released; }

        [[nodiscard]] bool Down() { return my.held; }
        [[nodiscard]] bool Hovered() const { return hovered; }

        [[nodiscard]] bool DoublePressed() const { return doublePressed; }

        [[nodiscard]] Vec2 DownAtOffset() {
            if (!Down()) {
                Log("[WARNING]: `DownAtOffset` called while Interactive not pressed! -- likely unintentional!");
                return { 0.0f, 0.0f };
            }

            auto prev = UsePrev();
            Vec2 mp = raylib::Mouse::GetPosition();
            return prev
                ? mp - prev->GetBorderedBounds().GetPosition()
                : mp;
        }

        [[nodiscard]] float DownAtCenterDist() {
            if (!Down()) {
                Log("[WARNING]: `DownAtOffset` called while Interactive not pressed! -- likely unintentional!");
                return 0.0f;
            }

            auto prev = UsePrev();
            Vec2 mp = raylib::Mouse::GetPosition();
            RectF rect = prev->GetBorderedBounds();
            Vec2 center = rect.GetPosition() + rect.GetSize() / 2.0f;

            return mp.Distance(center);
        }

        [[nodiscard]] float DownAtCenterAngle() {
            if (!Down()) {
                Log("[WARNING]: `DownAtOffset` called while Interactive not pressed! -- likely unintentional!");
                return 0.0f;
            }

            auto prev = UsePrev();
            Vec2 mp = raylib::Mouse::GetPosition();
            RectF rect = prev->GetBorderedBounds();
            Vec2 center = rect.GetPosition() + rect.GetSize() / 2.0f;

            return (mp - center).Angle({0, 0});
        }


        [[nodiscard]] Vec2 DownAtOffsetBound() {
            auto prev = UsePrev();
            Vec2 offset = DownAtOffset();

            if (!prev) { return offset; }

            auto[width, height] = prev->GetBorderedBounds().GetSize();

            return {
                std::clamp(offset.x, 0.0f, width),
                std::clamp(offset.y, 0.0f, height)
            };
        }

        [[nodiscard]] Vec2 DownAtNorm() {
            auto prev = UsePrev();
            Vec2 offset = DownAtOffset();

            return prev
                ? offset / Vec2(prev->width, prev->height)
                : offset;
        }

        [[nodiscard]] Vec2 DownAtBoundedNorm() {
            auto [x, y] = DownAtNorm();
            return {
                std::clamp(x, 0.0f, 1.0f),
                std::clamp(y, 0.0f, 1.0f)
            };
        }
    private:
        bool pressed = false;
        bool doublePressed = false;
        bool released = false;
        bool hovered = false;

        constexpr static float DOUBLE_PRESS_THRESHOLD = 0.5f;

        void OnClick() {
            pressed = true;
            DOM::Focus(id);

            // double press handling
            if (my.lastPressed && GetTime() - *my.lastPressed < DOUBLE_PRESS_THRESHOLD) {
                doublePressed = true;
                my.lastPressed = std::nullopt;
            } else {
                my.lastPressed = { GetTime() };
            }
        }
    };
}
