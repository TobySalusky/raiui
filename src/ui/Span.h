//
// Created by Tobiathan on 1/10/23.
//

#pragma once

#include "DOM.h"
#include "Style.h"
#include "Identification.h"

namespace tui {
    struct Span : UIComponent {

        explicit Span(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current())
                     : UIComponent(idLike, location) {

            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                    .flexDir=FlexDirection::Row
            }, style)});
        }

        ~Span() {
            DOM::CloseScope();
        }
    };
}