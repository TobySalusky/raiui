//
// Created by Tobiathan on 1/10/23.
//

#pragma once

#include "DOM.h"
#include "Style.h"
#include "Identification.h"

namespace tui {
    struct Span : OpenUIComponent {

        explicit Span(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current())
                     : OpenUIComponent(idLike, location) {

            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                    .flexDir=FlexDirection::Row
            }, style)});
        }
    };
}