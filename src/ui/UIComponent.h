//
// Created by Tobiathan on 2/1/23.
//

#pragma once

#include "Includes.h"

#include "Identification.h"
#include "ScopeId.h"
#include "Slot.h"
#include "State.h"


namespace tui {
    struct UIComponent {
        string id;

        UIComponent(const id_t& idLike, tloc location) : id(gen_id(idLike, location)) {}

        [[nodiscard]] optional_ref<UIElement> UsePrev() const {
            return DOM::Previous().Lookup(id);
        }

        [[nodiscard]] bool IsFocused() {
            if (!focused.has_value()) {
                focused = optional<bool> { DOM::FocusedId().has_value() && id == DOM::FocusedId().value() };
            }
            return focused.value();
        }

        void Focus() const { DOM::Focus(id); }
        void UnFocus() const { DOM::UnFocus(); }

    private:
        optional<bool> focused;
    };

    template <typename T>
    struct StateComponent : public UIComponent {
        T& my;
        StateComponent(const id_t& idLike, tloc location) : UIComponent(idLike, location), my(UseRef<T>(T{}, idLike, location)) {}
    };

    struct OpenUIComponent : public UIComponent {
        OpenUIComponent(const id_t& idLike, tloc location) : UIComponent(idLike, location) {}

        ~OpenUIComponent() {
            DOM::CloseScope();
        }
    };

    template <typename T>
    struct OpenStateComponent : public UIComponent {
        T& my;
        OpenStateComponent(const id_t& idLike, tloc location) : UIComponent(idLike, location), my(UseRef<T>(T{}, id_append{id})) {} // TODO: WHY DOES ADDING id CRASH??

        ~OpenStateComponent() {
            DOM::CloseScope();
        }
    };


//    struct ScopedComponent : public UIComponent {
//        ScopeId scopeId;
//        ScopedComponent(const id_t& idLike, tloc location)
//            : UIComponent(idLike, location), scopeId(id) {}
//    };

    template <typename T>
    concept ui_component = requires (T val) {
        { val.id } -> std::convertible_to<string>;
        { val.UsePrev() } -> std::convertible_to<optional_ref<UIElement>>;
    };
}
