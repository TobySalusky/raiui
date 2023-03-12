//
// Created by Tobiathan on 1/31/23.
//

#pragma once

#include "Includes.h"
#include "Style.h"
#include "Identification.h"
#include "DOM.h"
#include "Div.h"
#include "State.h"
#include "UIElement.h"

namespace tui {
    struct TooltipType : OpenUIComponent {
        explicit TooltipType(const style_t& style = {}, const id_t& idLike = "",
                         tloc location = tloc::current())
                     : OpenUIComponent(idLike, location) {
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
