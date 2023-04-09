#include <functional>
#include "UIElement.h"
#include "ColorConverter.h"

string tui::UIElement::RecursiveStringify() {
    string str = "'"s + id + "'";
    str += " ["s + std::to_string((int) width) + "," + std::to_string((int) height) + "]";
    if (!children.empty()) {
        str += " { ";
        for (auto& child : children) {
            str += child.RecursiveStringify() + ", ";
        }
        str += "}";
    }
    return str;
}

void tui::UIElement::Render(const RenderInfo& info) {
    const auto& [beginClip, endClip] = info;
    if (beginClip) { BeginClipRect(); }

    // draw background
    RenderBackground(); // TODO: only do if background is visible??/exists?

    // draw texture
    if (HasTexture()) {
        RenderTexture();
    }

    // draw border
    if (HasBorder()) {
        RenderBorder();
    }

    // draw text
    if (HasText()) {
        RenderText();
    }

    if (endClip) { EndClipRect(); }
}

void tui::UIElement::BuildUp() { // First Styling Pass
    ApplyStyle();
    ApplyAspectRatio();

    for (auto& child : children) {
        child.BuildUp();
    }

    SizeSelfByContent();
}

void tui::UIElement::BuildDown() { // Second Styling Pass
    RelativelySizeChildren();

    AlignChildren();
    JustifyChildren();
    ShiftRelativeChildren();
    // recursively build children
    for (auto& child : children) {
        child.BuildDown();
    }
}

void tui::UIElement::RelativelySizeChildren() {
    RelativelySizePercentageChildren();
    RelativelySizeFractionalChildren();
}

void tui::UIElement::RelativelySizePercentageChildren() {
    const auto contentAttachedChildren = ContentAttachedChildren();

    for (auto dir : { PRIMARY, SECONDARY }) {
        for (auto& child : contentAttachedChildren) {
            const auto ApplyPercentageStyle = [&](auto& styleDimen, float& dimenRef){
                if (auto perc = tutil::pick<percent>(styleDimen)) {
                    dimenRef = Dimen(dir) * perc->value / 100.0f;
                }
            };
            ApplyPercentageStyle(BorderWidthFirstStyle(dir, child), BorderWidthFirst(dir, child));
            ApplyPercentageStyle(MarginFirstStyle(dir, child), MarginFirst(dir, child));
            ApplyPercentageStyle(PaddingFirstStyle(dir, child), PaddingFirst(dir, child));
            ApplyPercentageStyle(DimenStyle(dir, child), Dimen(dir, child));
            ApplyPercentageStyle(PaddingLastStyle(dir, child), PaddingLast(dir, child));
            ApplyPercentageStyle(MarginLastStyle(dir, child), MarginLast(dir, child));
            ApplyPercentageStyle(BorderWidthLastStyle(dir, child), BorderWidthLast(dir, child));
        }
    }
    ApplyChildrenAspectRatios();
}

void tui::UIElement::RelativelySizeFractionalChildren() {
    const auto GetFractionalPortion = [&](bool dir, UIElement& el) {
        const auto ExtractFraction = [&](auto& size){
            return tutil::exists_with<fraction>(size) ? std::max(0.0f, std::get<fraction>(*size).value) : 0;
        };

        return ExtractFraction(BorderWidthFirstStyle(dir, el))
            + ExtractFraction(MarginFirstStyle(dir, el))
            + ExtractFraction(PaddingFirstStyle(dir, el))
            + ExtractFraction(DimenStyle(dir, el))
            + ExtractFraction(PaddingLastStyle(dir, el))
            + ExtractFraction(MarginLastStyle(dir, el))
            + ExtractFraction(BorderWidthLastStyle(dir, el));
    };

    const auto GetRealPortion = [&](bool dir, UIElement& el) {
        return TotalDimen(dir, el);
    };

    const auto ApplyFractionalStyles = [&](bool dir, UIElement& child, float perFractional){
        const auto ApplyFractionalStyle = [&](auto& styleDimen, float& dimenRef){
            if (auto frac = tutil::pick<fraction>(styleDimen)) {
                dimenRef = frac->value * perFractional;
            }
        };
        ApplyFractionalStyle(BorderWidthFirstStyle(dir, child), BorderWidthFirst(dir, child));
        ApplyFractionalStyle(MarginFirstStyle(dir, child), MarginFirst(dir, child));
        ApplyFractionalStyle(PaddingFirstStyle(dir, child), PaddingFirst(dir, child));
        ApplyFractionalStyle(DimenStyle(dir, child), Dimen(dir, child));
        ApplyFractionalStyle(PaddingLastStyle(dir, child), PaddingLast(dir, child));
        ApplyFractionalStyle(MarginLastStyle(dir, child), MarginLast(dir, child));
        ApplyFractionalStyle(BorderWidthLastStyle(dir, child), BorderWidthLast(dir, child));
    };

    const auto contentAttachedChildren = ContentAttachedChildren();

    { // TODO: FUNC? fractional along length
        const float sumFractional = tutil::transform_sum(contentAttachedChildren, [&](auto& child) { return GetFractionalPortion(PRIMARY, child); });

        if (sumFractional != 0.0f) {
            const float sumReal = tutil::transform_sum(contentAttachedChildren, [&](auto& child) { return GetRealPortion(PRIMARY, child); });
            const float availableLength = Dimen(PRIMARY) - sumReal;
            const float perFractional = availableLength / sumFractional;

            for (auto& child : contentAttachedChildren) {
                ApplyFractionalStyles(PRIMARY, child, perFractional);
            }
        }
    }
    { // TODO: FUNC? fractional along girth
        for (auto& child : contentAttachedChildren) {
            const float fractionalAmount = GetFractionalPortion(SECONDARY, child);

            if (fractionalAmount != 0.0f) { // do not divide by zero
                const float realAmount = GetRealPortion(SECONDARY, child);
                const float availableGirth = Dimen(SECONDARY) - realAmount;
                const float perFractional = availableGirth / fractionalAmount;

                ApplyFractionalStyles(SECONDARY, child, perFractional);
            }
        }
    }
    ApplyChildrenAspectRatios();
}

void tui::UIElement::ApplyAspectRatio() {
    if (!style.aspectRatio) { return; }
    const float aspectRatio = *style.aspectRatio;

    if (aspectRatio == 0.0f) { // TODO: should skip?
        Log("[STYLE-WARNING]: aspectRatio of 0... skipping!");
        return;
    }

    if (style.width && style.height) {         // >> CONTAIN within width & height

        const float maxHeight = std::min(width / aspectRatio, height);
        width = maxHeight * aspectRatio;
        height = maxHeight;

    } else if (style.width && !style.height) { // >> SCALE height to match width
        height = width / aspectRatio;
    } else if (!style.width && style.height) { // >> SCALE width to match height
        width = height * aspectRatio;
    } else {
        Log("[STYLE-WARNING]: aspectRatio used with no width or height specified");
    }
}

void tui::UIElement::ApplyChildrenAspectRatios() {
    for (auto& child : children) { child.ApplyAspectRatio(); }
}

void tui::UIElement::SizeSelfByContent() {
    // find dimensions if not specified by style
    const auto DimenStyle = [&](bool dir) -> optional<SizeVariant>& {
        return ((flexDir == FlexDirection::Column) ^ dir) ? style.height : style.width;
    };

    const auto contentAttachedChildren = ContentAttachedChildren();

    if (!DimenStyle(PRIMARY)) {
        Dimen(PRIMARY) = tutil::transform_sum(contentAttachedChildren, [&](UIElement& child) { return TotalDimen(PRIMARY, child); });
    }

    if (!DimenStyle(SECONDARY)) {
        Dimen(SECONDARY) = tutil::transform_max(contentAttachedChildren, [&](UIElement& child){ return TotalDimen(SECONDARY, child); });
    }
}

void tui::UIElement::JustifyChildren() {
    const float contentStart =
            Coord(PRIMARY) + MarginFirst(PRIMARY) + BorderWidthFirst(PRIMARY) + PaddingFirst(PRIMARY);

    const auto contentAttachedChildren = ContentAttachedChildren();

    // >> childLengthSum is capped at parent length
    const float childLengthSum = std::min(
            Dimen(PRIMARY),
            tutil::transform_sum(contentAttachedChildren, [&](UIElement& child) { return TotalDimen(PRIMARY, child); })
    );
    float runningOffset = 0.0f;

    switch (justifyContent) {
        case AlignType::SpaceEvenly: {
            const float gapLength = (Dimen(PRIMARY) - childLengthSum) / (float) (contentAttachedChildren.size() + 1);
            runningOffset += gapLength;
            for (auto &child: contentAttachedChildren) {
                Coord(PRIMARY, child) = contentStart + runningOffset;
                runningOffset += TotalDimen(PRIMARY, child) + gapLength;
            }
            break;
        }
        case AlignType::SpaceAround: {
            const float gapLength = (!contentAttachedChildren.empty())
                                    ? (Dimen(PRIMARY) - childLengthSum) / (float) contentAttachedChildren.size()
                                    : 0;
            runningOffset += gapLength / 2.0f;
            for (auto &child: contentAttachedChildren) {
                Coord(PRIMARY, child) = contentStart + runningOffset;
                runningOffset += TotalDimen(PRIMARY, child) + gapLength;
            }
            break;
        }
        case AlignType::SpaceBetween: {
            const float gapLength = (contentAttachedChildren.size() > 1)
                                    ? (Dimen(PRIMARY) - childLengthSum) / (float) (contentAttachedChildren.size() - 1)
                                    : 0;
            for (auto &child: contentAttachedChildren) {
                Coord(PRIMARY, child) = contentStart + runningOffset;
                runningOffset += TotalDimen(PRIMARY, child) + gapLength;
            }
            break;
        }
        case AlignType::End:
        case AlignType::FlexEnd:
            for (auto& child : contentAttachedChildren) {
                Coord(PRIMARY, child) = contentStart + (Dimen(PRIMARY) - childLengthSum) + runningOffset;
                runningOffset += TotalDimen(PRIMARY, child);
            }
            break;
        case AlignType::Center:
            for (auto& child : contentAttachedChildren) {
                Coord(PRIMARY, child) = contentStart + ((Dimen(PRIMARY) - childLengthSum) / 2.0f) + runningOffset;
                runningOffset += TotalDimen(PRIMARY, child);
            }
            break;
        case AlignType::Start:
        case AlignType::FlexStart:
        default:
            for (auto& child : contentAttachedChildren) {
                Coord(PRIMARY, child) = contentStart + runningOffset;
                runningOffset += TotalDimen(PRIMARY, child);
            }
            break;
    }
}

void tui::UIElement::AlignChildren() {
    const float contentStart =
        Coord(SECONDARY) + MarginFirst(SECONDARY) + BorderWidthFirst(SECONDARY) + PaddingFirst(SECONDARY);

    const auto contentAttachedChildren = ContentAttachedChildren();

    switch (alignItems) {
        case AlignType::Center:
            for (auto& child : contentAttachedChildren) {
                Coord(SECONDARY, child) = contentStart + (Dimen(SECONDARY) - TotalDimen(SECONDARY, child)) / 2.0f;
            }
            break;
        case AlignType::FlexEnd:
        case AlignType::End:
            for (auto& child : contentAttachedChildren) {
                Coord(SECONDARY, child) = contentStart + (Dimen(SECONDARY) - TotalDimen(SECONDARY, child));
            }
            break;
        case AlignType::FlexStart:
        case AlignType::Start:
        default:
            for (auto& child : contentAttachedChildren) {
                Coord(SECONDARY, child) = contentStart;
            }
            break;
    }
}



#define TUI_UI_ELEMENT_STYLE_STEAL(arg) if (style.arg) { arg = *style.arg; }
#define TUI_UI_ELEMENT_STYLE_STEAL_COLOR(arg) if (style.arg) { arg = ColorConverter::to_color(*style.arg); }
#define TUI_UI_ELEMENT_STYLE_STEAL_TOP_BOTTOM_LEFT_RIGHT(arg) if (style.arg) { arg ## Top = *style.arg; arg ## Bottom = *style.arg; arg ## Left = *style.arg; arg ## Right = *style.arg; }

#define TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(arg) if (style.arg) { if (std::holds_alternative<float>(*style.arg)) { arg = std::get<float>(*style.arg); } else if (std::holds_alternative<int>(*style.arg)) { arg = std::get<int>(*style.arg); } }

void tui::UIElement::ApplyStyle() {
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(width);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(height);

    TUI_UI_ELEMENT_STYLE_STEAL_COLOR(color);
    TUI_UI_ELEMENT_STYLE_STEAL_COLOR(backgroundColor);

    TUI_UI_ELEMENT_STYLE_STEAL(fontSize);
    TUI_UI_ELEMENT_STYLE_STEAL(fontStyle);

    TUI_UI_ELEMENT_STYLE_STEAL(flexDir);

    TUI_UI_ELEMENT_STYLE_STEAL(alignItems);
    TUI_UI_ELEMENT_STYLE_STEAL(justifyContent);

    TUI_UI_ELEMENT_STYLE_STEAL(overflow);

    TUI_UI_ELEMENT_STYLE_STEAL(position);

    if (style.left) { x = *style.left; }
    if (style.top) { y = *style.top; }

    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(paddingTop);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(paddingBottom);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(paddingLeft);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(paddingRight);

    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(marginTop);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(marginBottom);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(marginLeft);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(marginRight);

    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(borderTopWidth);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(borderBottomWidth);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(borderLeftWidth);
    TUI_UI_ELEMENT_STYLE_STEAL_TRIVIAL_SIZE_VARIANT(borderRightWidth);
    TUI_UI_ELEMENT_STYLE_STEAL_COLOR(borderColor);

    TUI_UI_ELEMENT_STYLE_STEAL(borderTopLeftRadius);
    TUI_UI_ELEMENT_STYLE_STEAL(borderTopRightRadius);
    TUI_UI_ELEMENT_STYLE_STEAL(borderBottomLeftRadius);
    TUI_UI_ELEMENT_STYLE_STEAL(borderBottomRightRadius);

    // NOTE: zIndex is handled later (`ClippingPreorderTraversal`)
}

RectF tui::UIElement::GetUnBorderedBounds() const {
    return {
        x + marginLeft + borderLeftWidth,
        y + marginTop + borderTopWidth,
        paddingLeft + width + paddingRight,
        paddingTop + height + paddingBottom
    };
}

RectF tui::UIElement::GetBorderedBounds() const {
    return {
        x + marginLeft,
        y + marginTop,
        borderLeftWidth + paddingLeft + width + paddingRight + borderRightWidth,
        borderTopWidth + paddingTop + height + paddingBottom + borderBottomWidth
    };
}

RectF tui::UIElement::GetClientBounds() {
    return {
        x, y,
        TotalDimen(WidthDir()),
        TotalDimen(HeightDir())
    };
}

RectF tui::UIElement::GetContentBounds() const {
    return {
        x + marginLeft + borderLeftWidth + paddingLeft,
        y + marginTop + borderTopWidth + paddingTop,
        width,
        height
    };
}

bool tui::UIElement::HasBorderRounding() const {
    return borderTopLeftRadius != 0.0f || borderTopRightRadius != 0.0f || borderBottomLeftRadius != 0.0f || borderBottomRightRadius != 0.0f;
}

void tui::UIElement::RenderBackground() const {
    RectF unBorderedBounds = GetUnBorderedBounds();

    if (HasBorderRounding()) {
        const float maxTopRad = std::max(borderTopLeftRadius, borderTopRightRadius);
        const float maxBottomRad = std::max(borderBottomLeftRadius, borderBottomRightRadius);

        const bool topLeftRound = borderTopLeftRadius != 0.0f;
        const bool topRightRound = borderTopRightRadius != 0.0f;
        const bool bottomLeftRound = borderBottomLeftRadius != 0.0f;
        const bool bottomRightRound = borderBottomRightRadius != 0.0f;

        const RectF primaryStrip = {
            unBorderedBounds.x,
            unBorderedBounds.y + maxTopRad,
            unBorderedBounds.width,
            unBorderedBounds.height - (maxTopRad + maxBottomRad)
        };
        primaryStrip.Draw(backgroundColor);

        if (topLeftRound || topRightRound) {
            const RectF middleTopStrip = {
                    unBorderedBounds.x + borderTopLeftRadius,
                    unBorderedBounds.y,
                    unBorderedBounds.width - (borderTopLeftRadius + borderTopRightRadius),
                    maxTopRad
            };
            middleTopStrip.Draw(backgroundColor);

            if (topLeftRound) {
                const RectF fillStrip = {
                    unBorderedBounds.x,
                    unBorderedBounds.y + borderTopLeftRadius,
                    borderTopLeftRadius,
                    primaryStrip.y - (unBorderedBounds.y + borderTopLeftRadius)
                };
                const Vec2 cornerOrigin = {
                    unBorderedBounds.x + borderTopLeftRadius,
                    unBorderedBounds.y + borderTopLeftRadius
                };
                fillStrip.Draw(backgroundColor);
                cornerOrigin.DrawCircle(borderTopLeftRadius, backgroundColor);
//                DrawCircleSector(cornerOrigin, borderTopLeftRadius, 0, PI / 2, 10, backgroundColor);
            }

            if (topRightRound) {
                const RectF fillStrip = {
                        middleTopStrip.x + middleTopStrip.width,
                    unBorderedBounds.y + borderTopRightRadius,
                    (unBorderedBounds.x + unBorderedBounds.width) - (middleTopStrip.x + middleTopStrip.width),
                    primaryStrip.y - (unBorderedBounds.y + borderTopRightRadius)
                };
                const Vec2 cornerOrigin = {
                    fillStrip.x,
                    unBorderedBounds.y + borderTopRightRadius
                };
                fillStrip.Draw(backgroundColor);
                cornerOrigin.DrawCircle(borderTopRightRadius, backgroundColor);
            }
        }

        if (bottomLeftRound || bottomRightRound) {
            const RectF middleBottomStrip = {
                unBorderedBounds.x + borderBottomLeftRadius,
                primaryStrip.y + primaryStrip.height,
                unBorderedBounds.width - (borderBottomLeftRadius + borderBottomRightRadius),
                maxBottomRad
            };
            middleBottomStrip.Draw(backgroundColor);

            if (bottomLeftRound) {
                const RectF fillStrip = {
                    unBorderedBounds.x,
                    primaryStrip.y + primaryStrip.height,
                    middleBottomStrip.x - unBorderedBounds.x,
                    maxBottomRad - borderBottomLeftRadius
                };
                const Vec2 cornerOrigin = {
                    unBorderedBounds.x + borderBottomLeftRadius,
                    unBorderedBounds.y + unBorderedBounds.height - borderBottomLeftRadius
                };
                fillStrip.Draw(backgroundColor);
                cornerOrigin.DrawCircle(borderBottomLeftRadius, backgroundColor);
            }

            if (bottomRightRound) {
                const RectF fillStrip = {
                        middleBottomStrip.x + middleBottomStrip.width,
                        primaryStrip.y + primaryStrip.height,
                        (unBorderedBounds.x + unBorderedBounds.width) - (middleBottomStrip.x + middleBottomStrip.width),
                        maxBottomRad - borderBottomRightRadius
                };
                const Vec2 cornerOrigin = {
                        fillStrip.x,
                        unBorderedBounds.y + unBorderedBounds.height - borderBottomRightRadius
                };
                fillStrip.Draw(backgroundColor);
                cornerOrigin.DrawCircle(borderBottomRightRadius, backgroundColor);
            }
        }
    } else {
        unBorderedBounds.Draw(backgroundColor);
    }
}

void tui::UIElement::RenderBorder() const {
    RectF unBorderedBounds = GetUnBorderedBounds();
    RectF borderBounds = GetBorderedBounds();
    RectF{ borderBounds.x, borderBounds.y, borderBounds.width, borderTopWidth }.Draw(borderColor);
    RectF{ borderBounds.x, borderBounds.y, borderLeftWidth, borderBounds.height }.Draw(borderColor);
    RectF{ borderBounds.x, unBorderedBounds.y + unBorderedBounds.height, borderBounds.width, borderBottomWidth }.Draw(borderColor);
    RectF{ unBorderedBounds.x + unBorderedBounds.width, borderBounds.y, borderRightWidth, borderBounds.height }.Draw(borderColor);
}

bool tui::UIElement::HasBorder() const {
    return borderLeftWidth != 0.0f || borderTopWidth != 0.0f || borderRightWidth != 0.0f || borderBottomWidth != 0.0f;
}

bool tui::UIElement::IsContentAttached() const {
    return position != PositionType::Fixed;
}

void tui::UIElement::ClippingPreorderTraversal
    (
        vector<UIElement*>& preorderTraversal,
        int parentZIndex,
        optional<RectF> parentClippingRect
    )
{
    // add to traversal
    preorderTraversal.push_back(this);

    // configure zIndex based on parent
    ZIndexVariant zIndexVariant = style.zIndex ? *style.zIndex : z_add{ 0 };
    if (auto zIndexExact = tutil::pick<int>(zIndexVariant)) {
        zIndex = *zIndexExact;
    } else if (auto zIndexRelative = tutil::pick<z_add>(zIndexVariant)) {
        zIndex = parentZIndex + zIndexRelative->diff;
    }

    // determine clipRect
    // NOTE: union of own clipRect and parents clip rect ignores parent when at higher zIndex)
    bool useParentClipRect = zIndex <= parentZIndex;

    clipRect = useParentClipRect ? parentClippingRect : std::nullopt;

    auto childClipRect = IntersectionRects(
            clipRect,GenOwnClipRect()
    );

    // recurse into children
    for (UIElement& child : children) {
        child.ClippingPreorderTraversal(preorderTraversal, zIndex, childClipRect);
    }
}

optional<RectF> tui::UIElement::GenOwnClipRect() {
    return HasPersonalClipping()
        ? optional<RectF>{ GetBorderedBounds() }
        : std::nullopt;
}

optional<RectF> tui::UIElement::IntersectionRects(const optional<RectF>& rect1, const optional<RectF>& rect2) {
    if (!rect1) { return rect2; }
    if (!rect2) { return rect1; }

    const float left = std::max(rect1->x, rect2->x);
    const float top = std::max(rect1->y, rect2->y);
    const float right = std::min(rect1->x + rect1->width, rect2->x + rect2->width);
    const float bottom = std::min(rect1->y + rect1->height, rect2->y + rect2->height);

    return RectF{ left, top, std::max(left, right) - left, std::max(top, bottom) - top };
}

void tui::UIElement::ShiftRelativeChildren() {
    // TODO: sus, cleanup?
    for (UIElement& child : children) {
        if (child.position != PositionType::Relative) { continue; }

        if (child.style.left) {
            child.x += *child.style.left;
        }
        if (child.style.top) {
            child.y += *child.style.top;
        }
    }
}

void tui::UIElement::BeginClipRect() {
    BeginScissorMode(
            (int) clipRect->x,(int) clipRect->y,
            (int) clipRect->width,(int) clipRect->height
    );
}

void tui::UIElement::EndClipRect() {
    EndScissorMode();
}

bool tui::UIElement::IsVisible() {
    return HasVisibleClipRect() && HasRenderables();
}

bool tui::UIElement::HasRenderables() const {
    // TODO: figure out if size is non-zero ?? for background/border??
    return HasText() || HasBorder() || HasBackground() || HasTexture();
}

bool tui::UIElement::HasVisibleClipRect() const {
    return !clipRect || (clipRect->width != 0 && clipRect->height != 0);
}

bool tui::UIElement::HasBackground() const {
    return (int) backgroundColor.GetA() != 0;
}

bool tui::UIElement::RectanglesMatch(const optional<RectF>& rect1, const optional<RectF>& rect2) {
    bool sameParity = rect1.has_value() == rect2.has_value();

    if (!sameParity) { return false; }
    if (!rect1.has_value()) { return true; }

    RectF r1 = *rect1;
    RectF r2 = *rect2;

    return r1.x == r2.x && r1.y == r2.y && r1.width == r2.width && r2.height == r2.height;
}

bool tui::UIElement::HasTexture() const {
    return tutil::exists_with<RayTexture*>(style.texture);
}

void tui::UIElement::RenderTexture() {
    RectF unBorderedBounds = GetUnBorderedBounds();

    RayTexture& texture = *std::get<RayTexture*>(*style.texture);
    texture.Draw(RectF { 0.0f, 0.0f, (float) texture.width, (float) texture.height }, unBorderedBounds);
}

bool tui::UIElement::HasText() const {
    return !text.empty();
}

void tui::UIElement::RenderText() const {
    RectF contentBounds = GetContentBounds();
    raylib::Text(Fonts::Get(fontStyle), text, fontSize, 0, color)
            .Draw((int) contentBounds.x, (int) contentBounds.y);
}

RectF tui::UIElement::GetVisibleBounds() const {
    return *IntersectionRects(GetBorderedBounds(), clipRect);
}

void tui::UIElement::AddRenderCall(const std::function<void(UIElement&)>& renderCall) {
    if (additionalRenderCalls) {
        additionalRenderCalls->push_back(renderCall);
    } else {
        additionalRenderCalls = { renderCall };
    }
}
