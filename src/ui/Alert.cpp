//
// Created by Tobiathan on 5/17/23.
//

#include "Alert.h"

void tui::ContentAlert::Trigger() {
    my.timeLeft = my.settings.maxTimeLeft;
}

optional<unique_ptr<tui::ZeroPoint>> tui::ContentAlert::Open() {
    if (my.timeLeft <= 0) {
        return std::nullopt;
    }

    return std::make_unique<ZeroPoint>(Style {
        .alignItems = AlignType::Center,
        .position = PositionType::Fixed,
        .left = my.pos.x,
        .top = my.pos.y,
        .zIndex = z_up,
    }, id);
}