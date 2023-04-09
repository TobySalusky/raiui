//
// Created by Tobiathan on 2/16/23.
//

#pragma once

#include <UIEssentials.h>
#include <UICommon.h>

namespace tui {
    struct ScrollViewY : public OpenUIComponent {
        explicit ScrollViewY(const style_t& outerStyle = {}, const style_t& innerStyle = {}, const id_t& idLike = "", tloc location = tloc::current())
                : OpenUIComponent(idLike, location) {
            Slot slot;
            ScopeId scopeId (id);

            // TODO: rewrite all into state struct
            // TODO: add clicking off scroller on scroll-bar to teleport it along height
            // state
            float& scrollY = UseRef(0.0f);

            struct ScrollBarGrabInfo { Vec2 grabbedAt; float grabbedScrollBarTop; };
            auto& scrollGrab = UseRef<optional<ScrollBarGrabInfo>>(std::nullopt);

            // container pre-definitions
            auto clipDef = predef();
            auto contentDef = predef();

            // update state
            optional_ref<UIElement> prevClipContainer = DOM::Previous().Lookup(clipDef.id);
            optional_ref<UIElement> prevContentContainer = DOM::Previous().Lookup(contentDef.id);

            bool scrollBar = false;
            float maxScroll = 0.0f;
            float scrollBarHeight = 0.0f;
            float scrollBarTopUnits = 0.0f;
            float scrollBarTop = 0.0f;
            if (prevClipContainer && prevContentContainer) { // TODO: make func
                float clipHeight = prevClipContainer->GetClientBounds().height;
                float contentHeight = prevContentContainer->GetClientBounds().height;
                if (contentHeight <= clipHeight) {
                    // no need for scrolling!
                    scrollY = 0.0f;
                } else {
                    // scrolling necessary...
                    maxScroll = contentHeight - clipHeight;
                    bool clipHovered = prevClipContainer->GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

                    if (clipHovered) {
                        scrollY -= raylib::Mouse::GetWheelMove() * SCROLL_VIEW_SCROLL_SPEED;
                    }

                    // constrain scroll to possible range
                    scrollY = std::clamp(scrollY, 0.0f, maxScroll);

                    // enable UI scroll bar
                    scrollBar = true;
                    scrollBarHeight = clipHeight * (clipHeight / contentHeight);
                    scrollBarTopUnits = (clipHeight - scrollBarHeight) / maxScroll;
                    scrollBarTop = scrollY * scrollBarTopUnits;
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

                if (scroller.Pressed()) {
                    scrollGrab = { raylib::Mouse::GetPosition(), scrollBarTop };
                }

                if (scroller.Released()) {
                    scrollGrab = std::nullopt;
                }

                if (scrollGrab) { // dragging scroll-bar
                    const float desiredTopDiff = raylib::Mouse::GetPosition().y - scrollGrab->grabbedAt.y;
                    const float desiredTop = scrollGrab->grabbedScrollBarTop + desiredTopDiff;
                    const float necessaryAndSufficientScrollY = desiredTop / scrollBarTopUnits;

                    scrollY = necessaryAndSufficientScrollY;

                    // constrain scroll to possible range
                    scrollY = std::clamp(scrollY, 0.0f, maxScroll);
                    // TODO: DRY please this is terrible!
                }
            } else {
                scrollGrab = std::nullopt;
            }
        }

    private:
        static constexpr float SCROLL_VIEW_SCROLL_SPEED = 5.0f;
    };
}