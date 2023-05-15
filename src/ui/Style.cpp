//
// Created by Tobiathan on 1/3/23.
//

#include "Style.h"
#include "Styles.h"
#include "CssParser.h"

style_t tui::to_style(const Style& style) {
    return style.Simplified();
}

style_t tui::to_style(const string& classList) {
    if (classList.find(' ') == string::npos) {
        return class_segment_to_style(classList);
    }
    Style style = {};
    for (auto sv : classList | str_split(' ')) {
        style.AddToTop(class_segment_to_style(sv));
    }
    return style.Simplified();
}

Style tui::class_segment_to_style(string_view classSegment) {
    // normal css class
    if (classSegment.find(':') == string::npos) {
        return Styles::Instance().Get(string{ classSegment });
    }

    // specifier class
    try {
        const auto [styleSpecifier, content] = dole<2>(
                classSegment
                | str_split(':')
                | MAP_FUNC(string) // TODO: expensive?
                | to_vector()
        );

        string cssContent;

        static unordered_map<string, string> simpleSpecifierTable = {
            { "w", "width" },
            { "h", "height" },
            { "dim", "dimen" },
            { "bg", "background-color" },
            { "border-c", "border-color" },
            { "border-w", "border-width" },
            { "rad", "border-radius" },
            { "p", "padding" },
            { "m", "margin" },
        };

        const auto AddCss = [&](const string& name, const string& propertyContent) {
            cssContent += name + ": " + propertyContent + ";";
        };

        if (simpleSpecifierTable.contains(styleSpecifier)) {
            AddCss(simpleSpecifierTable[styleSpecifier], content);
        } else if (styleSpecifier == "border") {
            const auto [borderWidth, borderColor] = dole<2>(
                    content
                    | str_split('-')
                    | MAP_FUNC(string)
                    | to_vector()
            );

            AddCss("border-width", borderWidth);
            AddCss("border-color", borderColor);
        } else {
            AddCss(styleSpecifier, content); // TODO: yuck
        }


        return CssParser::CssBodyToStyle(cssContent);

    } catch (...) {
        Log("[STYLE-WARNING]: Invalid style specifier: `{}`", classSegment);
    }
    return {};
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
