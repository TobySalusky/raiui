//
// Created by Tobiathan on 1/16/23.
//

#pragma once

#include <UIEssentials.h>
#include "DOM.h"

namespace tui {
    struct CenterPoint : OpenUIComponent {
        explicit CenterPoint(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current())
            : OpenUIComponent(idLike, location) {
            DOM::Current().Attach({.id = id, .style=combined_styles(Style { .dimen = 0.0f, .align = AlignType::Center }, style)});
        }
    };
}
