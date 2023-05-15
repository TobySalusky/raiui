//
// Created by Tobiathan on 3/2/23.
//

#include "Text.h"

void tui::Text(const string& text, const style_t& style, FontStyle fontStyle) {
    float fontSize = style.fontSize.value_or(DEFAULT_FONT_SIZE);
    auto textDimens = raylib::Text(Fonts::Get(fontStyle), text, fontSize).MeasureEx();

    style_t textStyle = combined_styles(
        Style{ .fontSize = fontSize, .fontStyle = fontStyle, .width = textDimens.x, .height = textDimens.y },
        style
    );

    DOM::Current().PopAttach({ .text = text, .style = textStyle });
}

void tui::Text(const strong& strongText, const style_t& style) {
    Text(strongText.text, style, FontStyle::Bold);
}

void tui::Text(const italic& strongText, const style_t& style) {
    Text(strongText.text, style, FontStyle::Italic);
}
