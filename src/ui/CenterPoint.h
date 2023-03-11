//
// Created by Tobiathan on 1/16/23.
//

#pragma once

#include "DOM.h"
#include "Style.h"
#include "Identification.h"
#include "UIComponent.h"

namespace tui {
    struct CenterPoint : UIComponent {
        explicit CenterPoint(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current())
            : UIComponent(idLike, location) {
            DOM::Current().Attach({.id = id, .style=combined_styles(Style { .dimen = 0.0f, .align = AlignType::Center }, style)});
        }

        ~CenterPoint() {
            DOM::CloseScope();
        }
    };
}
