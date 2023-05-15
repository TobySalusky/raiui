//
// Created by Tobiathan on 4/23/23.
//

#pragma once

#include <UIEssentials.h>

namespace tui {
    struct ZeroPoint : OpenUIComponent {
        explicit ZeroPoint(const style_t& style = {}, const id_t& idLike = "", tloc location = tloc::current()) : OpenUIComponent(idLike, location) {
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                .dimen = 0
            }, style)});
        }
    };
}