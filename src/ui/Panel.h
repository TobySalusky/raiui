//
// Created by Tobiathan on 1/14/23.
//

#ifndef ARTNET_PANEL_H
#define ARTNET_PANEL_H

#include "State.h"
#include "Identification.h"
#include "ScopeId.h"
#include "Div.h"
#include "Style.h"
#include "Button.h"
#include "Span.h"
#include "Slot.h"
#include <source_location>
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

    struct Panel : UIComponent {
        explicit Panel (const PanelOptions& options = {}, const style_t &style = {}, id_t&& idLike = "", const std::source_location &location = tloc::current())
                : UIComponent(idLike, location) {
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
                    const RectF prevRect = (**prev).GetVisibleBounds();
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

        ~Panel() {
            DOM::CloseScope();
        }
    };
}

#endif //ARTNET_PANEL_H
