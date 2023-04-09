//
// Created by Tobiathan on 1/14/23.
//

#pragma once

#include <UIEssentials.h>

#include "Div.h"
#include "Button.h"
#include "Span.h"

#include <algorithm>

namespace tui {

    enum class PanelDirection {
        Left, Right
    };

    struct PanelOptions {
        PanelDirection dir = PanelDirection::Right;
        float defaultWidth = 100.0f;
        float minWidth = 0.0f;
        float maxWidth = MAXFLOAT;
    };

    struct Panel : OpenUIComponent {
        explicit Panel (const PanelOptions& options = {}, const style_t &style = {}, id_t&& idLike = "", tloc location = tloc::current())
                : OpenUIComponent(idLike, location) {
            Slot slot;
            ScopeId scopeId (id);

            const auto[dir, defaultWidth, minWidth, maxWidth] = options;

            PersistState<float> s_width ([=]{ return defaultWidth; });

            FlexHoriz container (Style { .width = *s_width, .height = 100_pct });

            const auto PanelContentPane = [&]() {
                Div div (combined_styles(Style { .width = 100_pct, .height = 100_pct, .backgroundColor = GRAY }, style));
                slot.Here();
            };

            if (dir == PanelDirection::Right) { PanelContentPane(); }

            {
                Div dragBarContainer (Style { .width = 1, .height = 100_pct, .alignItems = AlignType::Center, .backgroundColor = BLACK });
                Button dragBar (content{}, Style { .width = 15, .height = 100_pct });
                auto prev = DOM::Previous().Lookup(container.id);
                if (dragBar.Down() && prev) {
                    const RectF prevRect = prev->GetBorderedBounds();
                    const float mouseX = (float) raylib::Mouse::GetX();
                    const float targetWidth = (dir == PanelDirection::Right)
                            ? mouseX - prevRect.x
                            : (prevRect.x + prevRect.width) - mouseX;

                    *s_width = std::clamp(targetWidth, minWidth, maxWidth);
                }
                // TODO: add mouse hover ?? to button as whole???
            }

            if (dir == PanelDirection::Left) { PanelContentPane(); }
        }
    };
}
