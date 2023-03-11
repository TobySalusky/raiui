//
// Created by Tobiathan on 12/26/22.
//

#ifndef ARTNET_BUTTON_H
#define ARTNET_BUTTON_H

#include "DOM.h"
#include "Text.h"
#include "Main.h"
#include "Style.h"
#include "Identification.h"
#include "State.h"
#include "UIComponent.h"

namespace tui {
    template<typename... Ts>
    struct content {
        explicit content(Ts... args) {
            contentList = { args... };
        }
        std::tuple<Ts...> contentList;

        template <size_t I = 0>
        constexpr void do_application()
        {
            if constexpr(I == sizeof...(Ts)) {
                return;
            } else {
                tui::Text(get<I>(contentList));
                do_application<I + 1>();
            }
        }
    };

    template<typename... Ts>
    struct spaced_content {
        explicit spaced_content(Ts... args) {
            contentList = { args... };
        }
        std::tuple<Ts...> contentList;

        template <size_t I = 0>
        constexpr void do_application()
        {
            if constexpr (I == sizeof...(Ts)) {
                return;
            } else if constexpr (I == 0) {
                tui::Text(get<I>(contentList));
                do_application<I + 1>();
            } else {
                tui::Text(" ");
                tui::Text(get<I>(contentList));
                do_application<I + 1>();
            }
        }
    };

    template<typename... Ts>
    void apply_content(content<Ts...> content) {
        content.do_application();
    }

    template<typename... Ts>
    void apply_content(spaced_content<Ts...> content) {
        content.do_application();
    }

    void apply_content(const auto& text) {
        tui::Text(text);
    }

    template <typename T>
    concept content_like = requires (T val) { // TODO: ?
        tui::apply_content(val);
    };


    struct Button : UIComponent {

        template<content_like content_t = content<>>
        explicit Button(const content_t& btnContent = content{}, const style_t& style = {}, const id_t& idLike = "", tloc location = tloc::current())
                : UIComponent(idLike, location) {

            bool& held = UseRef(false, id_append{ id });

            DOM::Current().Attach({.id = id, .style=style});
            apply_content(btnContent);

            if (auto prevRes = DOM::Previous().Lookup(id)) {
                UIElement& prev = **prevRes;
                hovered = prev.GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

                if (hovered && raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
                    pressed = true;
                    held = true;
                }

                if (raylib::Mouse::IsButtonReleased(MOUSE_BUTTON_LEFT) || !raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
                    released = true;
                    held = false;
                }

                down = held;
            }
        }

        ~Button() {
            DOM::CloseScope();
        }

        [[nodiscard]] bool Pressed() const { return pressed; }
        [[nodiscard]] bool Released() const { return released; }

        [[nodiscard]] bool Down() const { return down; }
        [[nodiscard]] bool Hovered() const { return hovered; }

    private:
        bool pressed = false;
        bool released = false;
        bool down = false;
        bool hovered = false;
    };

    template<content_like content_t>
    bool ButtonPress(const content_t& content, const style_t& style = {}, const id_t& idLike = "", tloc location = tloc::current()) {
        Button button(content, style, idLike, location);
        return button.Pressed();
    }

    template<content_like content_t>
    bool ButtonHover(const content_t& content, const style_t& style = {}, const id_t& idLike = "", tloc location = tloc::current()) {
        Button button(content, style, idLike, location);
        return button.Hovered();
    }
}

#endif //ARTNET_BUTTON_H
