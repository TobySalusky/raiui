//
// Created by Tobiathan on 2/16/23.
//

#ifndef ARTNET_SCROLLVIEWY_H
#define ARTNET_SCROLLVIEWY_H

#include "Includes.h"
#include "Identification.h"
#include "Style.h"
#include "UIComponent.h"
#include "Div.h"
#include "State.h"
#include "Span.h"
#include "Interactive.h"

namespace tui {
    struct ScrollViewY : public UIComponent {
        explicit ScrollViewY(const style_t& outerStyle = {}, const style_t& innerStyle = {}, const id_t& idLike = "", tloc location = tloc::current())
                : UIComponent(idLike, location) {
            Slot slot;
            ScopeId scopeId (id);

            // state
            float& scrollY = UseRef(0.0f);

            // container pre-definitions
            auto clipDef = predef();
            auto contentDef = predef();

            // update state
            optional<UIElement*> prevClipContainer = DOM::Previous().Lookup(clipDef.id);
            optional<UIElement*> prevContentContainer = DOM::Previous().Lookup(contentDef.id);

            bool scrollBar = false;
            float scrollBarHeight = 0.0f;
            float scrollBarTop = 0.0f;
            if (prevClipContainer && prevContentContainer) { // TODO: make func
                float clipHeight = (**prevClipContainer).GetClientBounds().height;
                float contentHeight = (**prevContentContainer).GetClientBounds().height;
                if (contentHeight <= clipHeight) {
                    // no need for scrolling!
                    scrollY = 0.0f;
                } else {
                    // scrolling necessary...
                    const float maxScroll = contentHeight - clipHeight;
                    bool clipHovered = (**prevClipContainer).GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

                    if (clipHovered) {
                        scrollY -= raylib::Mouse::GetWheelMove() * SCROLL_VIEW_SCROLL_SPEED;
                    }

                    // constrain scroll to possible range
                    scrollY = std::clamp(scrollY, 0.0f, maxScroll);

                    // enable UI scroll bar
                    scrollBar = true;
                    scrollBarHeight = clipHeight * (clipHeight / contentHeight);
                    scrollBarTop = (clipHeight - scrollBarHeight) * (scrollY / maxScroll);
                }
            }

            // render containers
            Span container("fill"); // sus TODO: change??
            {
                Div clipContainer (combined_styles(Style { .overflow = OverflowMode::Hidden }, outerStyle), clipDef);
                Div contentContainer (combined_styles(Style { .width = 1_fr, .position = PositionType::Relative, .top = -scrollY }, innerStyle), contentDef);
                slot.Here();
            }
            // render scroll-bar
            if (scrollBar) {
                Div scrollBarContainer(Style { .width = 10, .height = 1_fr, .alignItems = AlignType::Center, .borderWidth = 1, .borderColor = GRAY });
                // TODO: (the thing)!
                Interactive scroller(Style { .width = 1_fr, .height = scrollBarHeight, .backgroundColor = RayColor{100, 100, 100, 150}, .position = PositionType::Relative, .top = scrollBarTop });
            }
        }

        ~ScrollViewY() { // TODO: use inheritance (OpenUIComponent or something) to make this not necessary to replicate
            DOM::CloseScope();
        }

    private:
        static constexpr float SCROLL_VIEW_SCROLL_SPEED = 5.0f;
    };
}


#endif //ARTNET_SCROLLVIEWY_H
