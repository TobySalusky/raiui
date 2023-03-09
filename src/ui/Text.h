//
// Created by Tobiathan on 12/27/22.
//

#ifndef ARTNET_TEXT_H
#define ARTNET_TEXT_H

#include "Includes.h"
#include "DOM.h"
#include "../resources/Fonts.h"

namespace tui { // TODO: add std::fmt-like
    constexpr int DEFAULT_FONT_SIZE = 32; // TODO: make this not global??

    struct strong {
        string text;
    };

    struct italic {
        string text;
    };

    void Text(const string& text, const style_t& style = {}, FontStyle fontStyle = FontStyle::Normal);

    void Text(const strong& strongText, const style_t& style = {});

    void Text(const italic& strongText, const style_t& style = {});
}

#endif //ARTNET_TEXT_H
