//
// Created by Tobiathan on 1/22/23.
//

#include "CssParser.h"
#include <boost/algorithm/string.hpp>
#include "magic_enum.h"
#include "ColorConverter.h"
#include <cstring>



using namespace tutil;

vector<ClassStyle> CssParser::CssFileContentsToStyles(const string& contents) {
    // TODO: fix this, should combine all like types first! undo NamedStyle -> ClassStyle please, local hashmap?
    unordered_map<string, int> test = {};

    return contents
        | str_split('}')
        | drop_last()
        | reduce<std::unordered_map<string, Style>, string_view>({}, [](auto& map, auto sv){
            try {
                const auto [nameStr, body] = dole<2>(
                        sv
                        | str_split('{')
                        | MAP_FUNC(boost::algorithm::trim_copy)
                        | to_vector()
                );

                const auto names =
                        nameStr
                        | str_split(',')
                        | MAP_FUNC(boost::algorithm::trim_copy)
                        | FILTER_CALL(.starts_with('.'))
                        | MAP_CALL(.substr(1))
                        | MAP_FUNC(string)
                        | to_vector();

                const Style style = CssParser::CssBodyToStyle(body);

                for (const string& name : names) {
                    // >> must simplify style to avoid macro-like specifiers being lost
                    map[name].AddToTop(style.Simplified());
                }
            } catch (...) {
                Log("[Style-WARNING]: Failed to parse name or body of css style!");
            }
        })
        | transform([](auto& classStylePair){
            auto& [className, style] = classStylePair;
            return ClassStyle { className, style };
        })
        | to_vector();
}

Style CssParser::CssBodyToStyle(string_view css) {
    Style style = {};

    for (auto line :
            css
            | str_split_v(';')
            | MAP_FUNC(boost::algorithm::trim_copy)
            | FILTER_OUT_CALL(.empty())
    ) {
        try {
            ApplyCssLineToStyle(style, line);
        } catch (...) {
            Log("CSS line failed to apply: `{}`", line);
        }
    }

    return style;
}

void CssParser::ApplyCssLineToStyle(Style& style, string_view line) {
    auto [name, value] = dole<2>(
        line
        | str_split(':')
        | transform([](string_view sv){ return boost::algorithm::trim_copy(sv); })
        | filter([](string_view sv) { return !sv.empty(); })
        | to_vector()
    );

    const auto PascalCase = [](auto&& dashed){
        return dashed | str_split('-') | transform([](string_view sv){ return boost::to_upper_copy(""s + sv[0]) + string(sv.substr(1)); }) | join_str();
    };

    const auto TrimRightNonNum = [](string_view sv){
        return boost::algorithm::trim_right_copy_if(sv, [](char c){ return c < '0' || c > '9';});
    };

    const auto AsFloat = [](string_view sv){
        return std::stof(string(sv));
    };

    const auto AsInt = [](string_view sv){
        return std::stoi(string(sv));
    };

    const auto ParseAsString = [](string_view sv) -> string {
        if ((sv.starts_with('\'') && sv.ends_with('\'')) || (sv.starts_with('"') && sv.ends_with('"'))) {
            return string(sv.substr(1, sv.length() - 2));
        }
        throw std::runtime_error("invalid css string literals");
    };

    const auto ParseAsFloatOrPx = [&](string_view sv) {
        if (sv.ends_with("px")) {
            return AsFloat(TrimRightNonNum(sv));
        }
        return AsFloat(sv);
    };

    const auto ParseAsAspectRatio = [&](string_view sv) {
        if (sv.find('/') != string::npos) {
            const auto [x, y] = dole<2>(
                sv
                | str_split('/')
                | MAP_FUNC(boost::algorithm::trim_copy)
                | MAP_FUNC(AsFloat)
                | to_vector()
            );
            if (x == 0.0f || y == 0.0f) {
                Log("[STYLE-WARNING]: no zeros in aspect ratio (skipping...), found here: `{}`", sv);
                throw std::runtime_error("zero in aspect-ratio");
            }
            return x / y;
        }
        return AsFloat(sv);
    };

    const auto ParseAsSizeType = [&](string_view sv) -> SizeVariant {
        if (sv.ends_with("px")) {
            return AsFloat(TrimRightNonNum(sv));
        }
        if (sv.ends_with("fr")) {
            return tui::fraction { AsFloat(TrimRightNonNum(sv)) };
        }
        if (sv.ends_with("%")) {
            return tui::percent { AsFloat(TrimRightNonNum(sv)) };
        }
        if (sv.ends_with("vw")) {
            return tui::view_width_pct { AsFloat(TrimRightNonNum(sv)) };
        }
        if (sv.ends_with("vh")) {
            return tui::view_height_pct { AsFloat(TrimRightNonNum(sv)) };
        }
        return AsFloat(sv);
    };

    const auto TrySizeTypeApply = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = ParseAsSizeType(value); }
    };

    const auto TryFloatApply = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = AsFloat(value); }
    };

    const auto TryIntApply = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = AsInt(value); }
    };

    const auto TryFloatOrPxApply = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = ParseAsFloatOrPx(value); }
    };

    const auto TryAspectRatio = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = ParseAsAspectRatio(value); }
    };

    const auto TryColorApply = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = ColorConverter::to_color(string(value)); }
    };

    const auto TryStringApply = [&](const char* cssName, auto& paramSetter) {
        if (name == cssName) { paramSetter = ParseAsString(value); }
    };

    const auto TryEnumApply = [&]<typename EnumType>(const char* cssName, optional<EnumType>& paramSetter) {
        if (name != cssName) { return; }

        if (optional<EnumType> alignType = magic_enum::enum_cast<EnumType>(PascalCase(value))) {
            paramSetter = alignType;
        } else {
            Log("[STYLE-WARNING]: failed to parse `{}` as {}!", value, magic_enum::enum_type_name<EnumType>());
        }
    };

    // >> SIZES ===================
    TrySizeTypeApply("width", style.width);
    TrySizeTypeApply("height", style.height);
    TrySizeTypeApply("dimen", style.dimen);
    // TODO: dimens
    TrySizeTypeApply("padding", style.padding);
    TrySizeTypeApply("padding-horiz", style.paddingHoriz);
    TrySizeTypeApply("padding-vert", style.paddingVert);
    TrySizeTypeApply("padding-top", style.paddingTop);
    TrySizeTypeApply("padding-bottom", style.paddingBottom);
    TrySizeTypeApply("padding-left", style.paddingLeft);
    TrySizeTypeApply("padding-right", style.paddingRight);

    TrySizeTypeApply("margin", style.margin);
    TrySizeTypeApply("margin-horiz", style.marginHoriz);
    TrySizeTypeApply("margin-vert", style.marginVert);
    TrySizeTypeApply("margin-top", style.marginTop);
    TrySizeTypeApply("margin-bottom", style.marginBottom);
    TrySizeTypeApply("margin-left", style.marginLeft);
    TrySizeTypeApply("margin-right", style.marginRight);

    TrySizeTypeApply("border-width", style.borderWidth);
    TrySizeTypeApply("border-top-width", style.borderTopWidth);
    TrySizeTypeApply("border-bottom-width", style.borderBottomWidth);
    TrySizeTypeApply("border-left-width", style.borderLeftWidth);
    TrySizeTypeApply("border-right-width", style.borderRightWidth);

    // >> FLOATS =======================
    TryFloatApply("border-radius", style.borderRadius);
    TryFloatApply("border-top-left-radius", style.borderTopLeftRadius);
    TryFloatApply("border-top-right-radius", style.borderTopRightRadius);
    TryFloatApply("border-bottom-left-radius", style.borderBottomLeftRadius);
    TryFloatApply("border-bottom-right-radius", style.borderBottomRightRadius);

    TryFloatApply("left", style.left);
    TryFloatApply("top", style.top);

    // >> FLOATS/PXs ===================
    TryFloatOrPxApply("font-size", style.fontSize);

    // >> INTS =========================
    TryIntApply("z-index", style.zIndex); // TODO: add relative zIndex?
    TryIntApply("tab-index", style.tabIndex);

    // >> ASPECT RATIO ===============
    TryAspectRatio("aspect-ratio", style.aspectRatio);

    // >> COLOURS ====================
    TryColorApply("background-color", style.backgroundColor);
    TryColorApply("border-color", style.borderColor);
    TryColorApply("color", style.color);

    // >> Align Types =================
    TryEnumApply("align", style.align);
    TryEnumApply("align-items", style.alignItems);
    TryEnumApply("justify-content", style.justifyContent);

    // >> Flex Direction ===============
    TryEnumApply("flex-direction", style.flexDir);

    // >> Overflow Type ======================
    TryEnumApply("overflow", style.overflow);

    // >> Position Type ======================
    TryEnumApply("position", style.position);

    // >> STRINGS ======================
    TryStringApply("texture", style.texture);
}