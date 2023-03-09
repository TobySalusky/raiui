//
// Created by Tobiathan on 1/10/23.
//

#ifndef ARTNET_SPAN_H
#define ARTNET_SPAN_H

#include "DOM.h"
#include "Style.h"
#include "Identification.h"
#include <source_location>

namespace tui {
    struct Span : UIComponent {

        explicit Span(const style_t &style = {}, const id_t& idLike = "",
                     const std::source_location &location = tloc::current())
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

#endif //ARTNET_SPAN_H
