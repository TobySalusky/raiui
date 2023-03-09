//
// Created by Tobiathan on 1/3/23.
//

#include "Style.h"
#include "Styles.h"

style_t tui::to_style(const Style& style) {
    return style.Simplified();
}

style_t tui::to_style(const string& classList) {
    if (classList.find(' ') == string::npos) {
        return Styles::Instance().Get(classList);
    }
    Style style = {};
    for (auto sv : classList | str_split(' ')) {
        style.AddToTop(
            Styles::Instance().Get(string{ sv })
        );
    }
    return style.Simplified();
}


style_t tui::to_style(const ClassAndStyle& classListAndStyle) { // TODO: add styleAndClassList overload?
    const auto& [classList, style] = classListAndStyle;
    return combined_styles(classList, style);
}

style_t tui::combined_styles(const style_t& s1, const style_t& s2) {
    style_t style = s1;
    style.AddToTop(s2);

    return style;
}

//ClassAndStyle::operator Style() const {
//    return tui::to_style(*this);
//}
