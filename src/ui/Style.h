//
// Created by Tobiathan on 1/3/23.
//

#pragma once

#include "Includes.h"
#include "Units.h"
#include "../resources/Fonts.h"
#include "Main.h"

#define TUI_STYLE_STEAL(arg) if (other.arg) { arg = other.arg; }

// TODO: add FlexDirection ColumnReverse and RowReverse (then FlexStart/FlexEnd work for reverse's as well [Start/End will not])
enum class AlignType {
    FlexStart, FlexEnd, Start, End, Center, SpaceBetween, SpaceAround, SpaceEvenly
};

enum class FlexDirection {
    Column, Row
};

enum class OverflowMode {
    Visible, Hidden, Scroll
};

enum class PositionType {
    Auto, Fixed, Relative
};

struct z_add {
    int diff;
};

#define z_up z_add{1}
#define z_down z_add{-1}

using ZIndexVariant = variant<int, z_add>;
using SizeVariant = variant<float, int, tui::percent, tui::fraction>;
using ColorVariant = variant<RayColor, string>;
using TextureVariant = variant<RayTexture*, string>;

struct Dimensions {
    SizeVariant width {};
    SizeVariant height {};
};

class Style {
public:
    optional<float> fontSize = std::nullopt;
    optional<FontStyle> fontStyle = std::nullopt;

    optional<SizeVariant> dimen = std::nullopt;
    optional<Dimensions> dimens = std::nullopt;
    optional<SizeVariant> width = std::nullopt;
    optional<SizeVariant> height = std::nullopt;

    // >> aspectRatio applies only to width/height (not padding or margin)
    optional<float> aspectRatio = std::nullopt;

    optional<FlexDirection> flexDir = std::nullopt;

    optional<AlignType> align = std::nullopt;
    optional<AlignType> justifyContent = std::nullopt;
    optional<AlignType> alignItems = std::nullopt;

    optional<SizeVariant> padding = std::nullopt;
    optional<SizeVariant> paddingHoriz = std::nullopt;
    optional<SizeVariant> paddingVert = std::nullopt;
    optional<SizeVariant> paddingTop = std::nullopt;
    optional<SizeVariant> paddingBottom = std::nullopt;
    optional<SizeVariant> paddingLeft = std::nullopt;
    optional<SizeVariant> paddingRight = std::nullopt;

    optional<SizeVariant> margin = std::nullopt;
    optional<SizeVariant> marginHoriz = std::nullopt;
    optional<SizeVariant> marginVert = std::nullopt;
    optional<SizeVariant> marginTop = std::nullopt;
    optional<SizeVariant> marginBottom = std::nullopt;
    optional<SizeVariant> marginLeft = std::nullopt;
    optional<SizeVariant> marginRight = std::nullopt;

    optional<SizeVariant> borderWidth = std::nullopt;
    optional<SizeVariant> borderTopWidth = std::nullopt;
    optional<SizeVariant> borderBottomWidth = std::nullopt;
    optional<SizeVariant> borderLeftWidth = std::nullopt;
    optional<SizeVariant> borderRightWidth = std::nullopt;

    optional<ColorVariant> borderColor = std::nullopt;

    optional<float> borderRadius = std::nullopt;
    optional<float> borderTopLeftRadius = std::nullopt;
    optional<float> borderTopRightRadius = std::nullopt;
    optional<float> borderBottomLeftRadius = std::nullopt;
    optional<float> borderBottomRightRadius = std::nullopt;

    optional<ColorVariant> color = std::nullopt;
    optional<ColorVariant> backgroundColor = std::nullopt;

    optional<TextureVariant> texture = std::nullopt;

    // TODO: box-shadow

    optional<OverflowMode> overflow = std::nullopt;
    optional<PositionType> position = std::nullopt;

    optional<float> left = std::nullopt;
    optional<float> top = std::nullopt;

    optional<ZIndexVariant> zIndex = std::nullopt;

//    optional<OverflowMode> overflowX = std::nullopt; // TODO:
//    optional<OverflowMode> overflowY = std::nullopt;

    [[nodiscard]] Style Simplified() const {
        Style style;

        if (padding) {
            style.paddingTop = padding;
            style.paddingBottom = padding;
            style.paddingLeft = padding;
            style.paddingRight = padding;
        }

        if (paddingHoriz) {
            style.paddingLeft = paddingHoriz;
            style.paddingRight = paddingHoriz;
        }

        if (paddingVert) {
            style.paddingTop = paddingVert;
            style.paddingBottom = paddingVert;
        }

        if (margin) {
            style.marginTop = margin;
            style.marginBottom = margin;
            style.marginLeft = margin;
            style.marginRight = margin;
        }

        if (marginHoriz) {
            style.marginLeft = marginHoriz;
            style.marginRight = marginHoriz;
        }

        if (marginVert) {
            style.marginTop = marginVert;
            style.marginBottom = marginVert;
        }

        if (borderWidth) {
            style.borderTopWidth = borderWidth;
            style.borderBottomWidth = borderWidth;
            style.borderLeftWidth = borderWidth;
            style.borderRightWidth = borderWidth;
        }

        if (borderRadius) {
            style.borderTopLeftRadius = borderRadius;
            style.borderTopRightRadius = borderRadius;
            style.borderBottomLeftRadius = borderRadius;
            style.borderBottomRightRadius = borderRadius;
        }

        if (dimen) {
            style.width = dimen;
            style.height = dimen;
        }

        if (dimens) {
            style.width = dimens->width;
            style.height = dimens->height;
        }

        if (align) {
            style.justifyContent = align;
            style.alignItems = align;
        }

//        if (overflow) {
//            style.overflowX = overflow;
//            style.overflowY = overflow;
//        }

        style.AddToTop(*this);

        if (texture) {
            style.texture =
                    std::holds_alternative<string>(*texture)
                    ? tui::Main::LookupTextureFromString(std::get<string>(*texture))
                    : std::get<RayTexture*>(*texture);
        }

        return style;
    }

    void AddToTop(const Style& other) {
        TUI_STYLE_STEAL(fontSize);
        TUI_STYLE_STEAL(fontStyle);

        TUI_STYLE_STEAL(width);
        TUI_STYLE_STEAL(height);
        TUI_STYLE_STEAL(aspectRatio);

        TUI_STYLE_STEAL(flexDir);
        TUI_STYLE_STEAL(justifyContent);
        TUI_STYLE_STEAL(alignItems);

        TUI_STYLE_STEAL(paddingTop);
        TUI_STYLE_STEAL(paddingBottom);
        TUI_STYLE_STEAL(paddingLeft);
        TUI_STYLE_STEAL(paddingRight);

        TUI_STYLE_STEAL(marginTop);
        TUI_STYLE_STEAL(marginBottom);
        TUI_STYLE_STEAL(marginLeft);
        TUI_STYLE_STEAL(marginRight);

        TUI_STYLE_STEAL(borderTopWidth);
        TUI_STYLE_STEAL(borderBottomWidth);
        TUI_STYLE_STEAL(borderLeftWidth);
        TUI_STYLE_STEAL(borderRightWidth);

        TUI_STYLE_STEAL(borderColor);

        TUI_STYLE_STEAL(borderTopLeftRadius);
        TUI_STYLE_STEAL(borderTopRightRadius);
        TUI_STYLE_STEAL(borderBottomLeftRadius);
        TUI_STYLE_STEAL(borderBottomRightRadius);

        TUI_STYLE_STEAL(color);
        TUI_STYLE_STEAL(backgroundColor);

        TUI_STYLE_STEAL(texture);

        // TODO: instead, use overflowX & overflowY
        TUI_STYLE_STEAL(overflow);
        TUI_STYLE_STEAL(zIndex);

        TUI_STYLE_STEAL(position);

        TUI_STYLE_STEAL(left);
        TUI_STYLE_STEAL(top);
    }
};

struct ClassAndStyle {
    string classes;
    Style style;
};
using CStyle = ClassAndStyle;


//template <typename T>
//concept style_like = requires (T val) {
//    tui::to_style(val);
//};

struct style_t;

namespace tui {
    style_t to_style(const Style& style);

    style_t to_style(const string& classList);

    Style class_segment_to_style(string_view classSegment);

    style_t to_style(const ClassAndStyle& classListAndStyle);

    style_t combined_styles(const style_t& s1, const style_t& s2);
}

struct style_t {
    style_t() = default;

    style_t(const Style& style) {
        Style simplified = style.Simplified();

        fontSize = simplified.fontSize;
        fontStyle = simplified.fontStyle;
        width = simplified.width;
        height = simplified.height;
        aspectRatio = simplified.aspectRatio;
        flexDir = simplified.flexDir;
        justifyContent = simplified.justifyContent;
        alignItems = simplified.alignItems;
        paddingTop = simplified.paddingTop;
        paddingBottom = simplified.paddingBottom;
        paddingLeft = simplified.paddingLeft;
        paddingRight = simplified.paddingRight;
        marginTop = simplified.marginTop;
        marginBottom = simplified.marginBottom;
        marginLeft = simplified.marginLeft;
        marginRight = simplified.marginRight;
        borderTopWidth = simplified.borderTopWidth;
        borderBottomWidth = simplified.borderBottomWidth;
        borderLeftWidth = simplified.borderLeftWidth;
        borderRightWidth = simplified.borderRightWidth;
        borderColor = simplified.borderColor;
        borderTopLeftRadius = simplified.borderTopLeftRadius;
        borderTopRightRadius = simplified.borderTopRightRadius;
        borderBottomLeftRadius = simplified.borderBottomLeftRadius;
        borderBottomRightRadius = simplified.borderBottomRightRadius;
        color = simplified.color;
        backgroundColor = simplified.backgroundColor;
        texture = simplified.texture;
        overflow = simplified.overflow;
        position = simplified.position;
        left = simplified.left;
        top = simplified.top;
        zIndex = simplified.zIndex;
    }

    void AddToTop(const style_t& other) {
        TUI_STYLE_STEAL(fontSize);
        TUI_STYLE_STEAL(fontStyle);

        TUI_STYLE_STEAL(width);
        TUI_STYLE_STEAL(height);
        TUI_STYLE_STEAL(aspectRatio);

        TUI_STYLE_STEAL(flexDir);
        TUI_STYLE_STEAL(justifyContent);
        TUI_STYLE_STEAL(alignItems);

        TUI_STYLE_STEAL(paddingTop);
        TUI_STYLE_STEAL(paddingBottom);
        TUI_STYLE_STEAL(paddingLeft);
        TUI_STYLE_STEAL(paddingRight);

        TUI_STYLE_STEAL(marginTop);
        TUI_STYLE_STEAL(marginBottom);
        TUI_STYLE_STEAL(marginLeft);
        TUI_STYLE_STEAL(marginRight);

        TUI_STYLE_STEAL(borderTopWidth);
        TUI_STYLE_STEAL(borderBottomWidth);
        TUI_STYLE_STEAL(borderLeftWidth);
        TUI_STYLE_STEAL(borderRightWidth);

        TUI_STYLE_STEAL(borderColor);

        TUI_STYLE_STEAL(borderTopLeftRadius);
        TUI_STYLE_STEAL(borderTopRightRadius);
        TUI_STYLE_STEAL(borderBottomLeftRadius);
        TUI_STYLE_STEAL(borderBottomRightRadius);

        TUI_STYLE_STEAL(color);
        TUI_STYLE_STEAL(backgroundColor);

        TUI_STYLE_STEAL(texture);

        // TODO: instead, use overflowX & overflowY
        TUI_STYLE_STEAL(overflow);
        TUI_STYLE_STEAL(zIndex);

        TUI_STYLE_STEAL(position);

        TUI_STYLE_STEAL(left);
        TUI_STYLE_STEAL(top);
    }

    style_t(const char* classList) : style_t(string(classList)) {}
    style_t(const string& classList) : style_t(tui::to_style(classList)) {}
    style_t(const CStyle& classAndStyle) : style_t(tui::to_style(classAndStyle)) {}

    optional<float> fontSize = std::nullopt;
    optional<FontStyle> fontStyle = std::nullopt;

    optional<SizeVariant> width = std::nullopt;
    optional<SizeVariant> height = std::nullopt;

    optional<float> aspectRatio = std::nullopt;

    optional<FlexDirection> flexDir = std::nullopt;

    optional<AlignType> justifyContent = std::nullopt;
    optional<AlignType> alignItems = std::nullopt;

    optional<SizeVariant> paddingTop = std::nullopt;
    optional<SizeVariant> paddingBottom = std::nullopt;
    optional<SizeVariant> paddingLeft = std::nullopt;
    optional<SizeVariant> paddingRight = std::nullopt;

    optional<SizeVariant> marginTop = std::nullopt;
    optional<SizeVariant> marginBottom = std::nullopt;
    optional<SizeVariant> marginLeft = std::nullopt;
    optional<SizeVariant> marginRight = std::nullopt;

    optional<SizeVariant> borderTopWidth = std::nullopt;
    optional<SizeVariant> borderBottomWidth = std::nullopt;
    optional<SizeVariant> borderLeftWidth = std::nullopt;
    optional<SizeVariant> borderRightWidth = std::nullopt;

    optional<ColorVariant> borderColor = std::nullopt;

    optional<float> borderTopLeftRadius = std::nullopt;
    optional<float> borderTopRightRadius = std::nullopt;
    optional<float> borderBottomLeftRadius = std::nullopt;
    optional<float> borderBottomRightRadius = std::nullopt;

    optional<ColorVariant> color = std::nullopt;
    optional<ColorVariant> backgroundColor = std::nullopt;

    optional<TextureVariant> texture = std::nullopt;

    optional<OverflowMode> overflow = std::nullopt;
    optional<PositionType> position = std::nullopt;

    optional<float> left = std::nullopt;
    optional<float> top = std::nullopt;

    optional<ZIndexVariant> zIndex = std::nullopt;
};
