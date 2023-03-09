//
// Created by Tobiathan on 1/31/23.
//

#ifndef ARTNET_TOOLTIP_H
#define ARTNET_TOOLTIP_H

#include "Includes.h"
#include "Style.h"
#include "Identification.h"
#include "DOM.h"
#include "Div.h"
#include "State.h"
#include "UIElement.h"

namespace tui {
    struct TooltipType : UIComponent {
        explicit TooltipType(const style_t& style = {}, const id_t& idLike = "",
                         tloc location = tloc::current())
                     : UIComponent(idLike, location) {
//            Vec2 mousePos = raylib::Mouse::GetPosition();
            DOM::Current().Attach(
                {
                    .id = id,
                    .style=combined_styles(
                        Style {
                            .position = PositionType::Fixed,
                            .zIndex = z_up
                        },
                        style
                    )
                }
            );
        }

        ~TooltipType() {
            DOM::CloseScope();
        }
    };

    optional<unique_ptr<TooltipType>> TooltipForId(const string& targetId, const id_t& idLike = "", tloc location = tloc::current());

    optional<unique_ptr<TooltipType>> Tooltip(ui_component auto const& target, const id_t& idLike = "", tloc location = tloc::current()) {
        return TooltipForId(target.id, idLike, location);
    }

    optional<unique_ptr<TooltipType>> DelayTooltipForId(const string& targetId, float delay = 0.5f, const id_t& idLike = "", tloc location = tloc::current());

    optional<unique_ptr<TooltipType>> DelayTooltip(ui_component auto const& target, float delay = 0.5f, const id_t& idLike = "", tloc location = tloc::current()) {
        return DelayTooltipForId(target.id, delay, idLike, location);
    }
}
#endif //ARTNET_TOOLTIP_H
