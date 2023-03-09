//
// Created by Tobiathan on 1/10/23.
//

#ifndef ARTNET_DIV_H
#define ARTNET_DIV_H

#include "DOM.h"
#include "Style.h"
#include "Identification.h"
#include "UIComponent.h"
#include <source_location>

namespace tui {
    struct Div : UIComponent {

        explicit Div(const style_t &style = {}, const id_t& idLike = "",
                          const std::source_location &location = tloc::current())
                          : UIComponent(idLike, location) {
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {.flexDir=FlexDirection::Column}, style)});
        }

        ~Div() {
            DOM::CloseScope();
        }
    };
}


#endif //ARTNET_DIV_H
