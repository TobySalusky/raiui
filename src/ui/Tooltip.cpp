//
// Created by Tobiathan on 1/31/23.
//

#include "Tooltip.h"

optional<unique_ptr<tui::TooltipType>> tui::TooltipForId(const string& targetId, const id_t& idLike, tloc location) {
    if (auto prev = DOM::Previous().Lookup(targetId)) {
        bool hovered = prev->GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

        if (hovered) { return { std::make_unique<TooltipType>(Style{ .left = raylib::Mouse::GetPosition().x, .top = raylib::Mouse::GetPosition().y + 20 }, idLike, location) }; }
    }
    return std::nullopt;
}



optional<unique_ptr<tui::TooltipType>> tui::DelayTooltipForId(const string& targetId, float delay, const id_t& idLike, tloc location) {
    if (auto prev = DOM::Previous().Lookup(targetId)) {
        float& hoveredFor = UseRef(0.0f, id_append { targetId });
        Vec2& hoverAtAnchor = UseRef(Vec2{}, id_append { targetId });

        bool hovered = prev->GetVisibleBounds().CheckCollision(raylib::Mouse::GetPosition()); // TODO: interactive-rect

        if (hovered) {
            hoveredFor += GetFrameTime();
            if (hoveredFor >= delay) {
                return { std::make_unique<TooltipType>(Style{ .left = hoverAtAnchor.x, .top = hoverAtAnchor.y + 20 }, idLike, location) };
            } else {
                hoverAtAnchor = raylib::Mouse::GetPosition();
            }
        } else {
            hoveredFor = 0.0f;
        }
    }
    return std::nullopt;
}