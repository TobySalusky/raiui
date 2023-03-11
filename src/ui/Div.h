//
// Created by Tobiathan on 1/10/23.
//

#ifndef ARTNET_DIV_H
#define ARTNET_DIV_H

#include "Includes.h"
#include "DOM.h"
#include "Style.h"
#include "Identification.h"
#include "UIComponent.h"

namespace tui {
    struct Div : UIComponent {

        explicit Div(const style_t &style = {}, const id_t& idLike = "",
                     tloc location = tloc::current())
                          : UIComponent(idLike, location) {
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {.flexDir=FlexDirection::Column}, style)});
        }

        ~Div() {
            DOM::CloseScope();
        }
    };
}


#endif //ARTNET_DIV_H
