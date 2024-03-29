//
// Created by Tobiathan on 3/7/23.
//

#pragma once

#include "Includes.h"
#include "Style.h"

struct ChoiceStyle {
    bool active;
    style_t s;
};

class OptStyles {
public:
    explicit OptStyles(const style_t& base, std::initializer_list<ChoiceStyle> opts = {}) {
        generatedStyle = base;
        for (auto& [shouldApply, style] : opts | reverse) { // TODO: bit sus...
            if (shouldApply) { generatedStyle.AddToTop(style); }
        }
    }

    operator style_t() {
        return generatedStyle;
    }
private:
    style_t generatedStyle;
};
