//
// Created by Tobiathan on 2/1/23.
//

#pragma once

#include "Includes.h"

#include "Identification.h"
#include "ScopeId.h"
#include "Slot.h"


namespace tui {
    struct UIComponent {
        string id;

        UIComponent(const id_t& idLike, tloc location) : id(gen_id(idLike, location)) {}

        [[nodiscard]] optional_ref<UIElement> UsePrev() const {
            return DOM::Previous().Lookup(id);
        }
    };

    struct OpenUIComponent : public UIComponent {
        OpenUIComponent(const id_t& idLike, tloc location) : UIComponent(idLike, location) {}

        ~OpenUIComponent() {
            DOM::CloseScope();
        }
    };

    struct ScopedComponent : public UIComponent {
        ScopeId scopeId;
        ScopedComponent(const id_t& idLike, tloc location)
            : UIComponent(idLike, location), scopeId(id) {}
    };

    template <typename T>
    concept ui_component = requires (T val) {
        { val.id } -> std::convertible_to<string>;
        { val.UsePrev() } -> std::convertible_to<optional_ref<UIElement>>;
    };
}
