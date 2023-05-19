//
// Created by Tobiathan on 5/17/23.
//

#pragma once
#include <UIEssentials.h>
#include <UICommon.h>
#include "ZeroPoint.h"

namespace tui {

    struct ContentAlertSettings {
        float maxTimeLeft = 1.0f;
        float padding = 50.0f;
    };

    struct ContentAlertState {
        float timeLeft = 0.0f;
        Vec2 pos = {};

        ContentAlertSettings settings = {};
    };

    struct ContentAlert : StateComponent<ContentAlertState> {
        ContentAlert(ui_component auto const& target, const ContentAlertSettings& settings = {}, const id_t& idLike = "", tloc location = tloc::current())
                : StateComponent<ContentAlertState>(idLike, location) {
            my.settings = settings;
            my.timeLeft -= GetFrameTime();
            auto prev = target.UsePrev();
            if (prev) {
                RectF prevRect = prev->GetVisibleBounds();
                my.pos = prevRect.GetPosition() + prevRect.GetSize() * Vec2{ 1.0f, 0.5f } + Vec2{ my.settings.padding, 0 };
            }
        }

        void Trigger();
        optional<unique_ptr<ZeroPoint>> Open();
    };

    struct TextAlert : ContentAlert {
        TextAlert(ui_component auto const& target, const string& text, const style_t& style = {}, const ContentAlertSettings& settings = {}, const id_t& idLike = "", tloc location = tloc::current())
                : ContentAlert(target, settings, idLike, location) {
            if (auto alertOpen = Open()) {
                Text(text, style);
            }
        }
    };
}