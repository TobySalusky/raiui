#pragma once

#include <list>
#include "Includes.h"
#include "Style.h"

#define PRIMARY false
#define SECONDARY true

namespace tui {
    using namespace tutil;

    class UIElement {
    public: // NOTE: actual public
        string RecursiveStringify();

        void BuildUp();
        void BuildDown();

        struct RenderInfo { bool beginClip; bool endClip; };
        void Render(const RenderInfo& info);

        [[nodiscard]] RectF GetClientBounds();
        [[nodiscard]] RectF GetContentBounds() const;
        [[nodiscard]] RectF GetUnBorderedBounds() const;
        [[nodiscard]] RectF GetBorderedBounds() const;
        [[nodiscard]] RectF GetVisibleBounds() const;

        void ClippingPreorderTraversal(std::vector<UIElement*>& preorderTraversal, int parentZIndex = 0, optional<RectF> parentClippingRect = std::nullopt);

        bool IsVisible(); // determine whether to render element
        static bool RectanglesMatch(const optional<RectF>& rect1, const optional<RectF>& rect2);

        void AddRenderCall(const std::function<void(UIElement&)>& renderCall);

    public: // TODO: make private
        string id;

        string text;
        float fontSize = 80;
        FontStyle fontStyle = FontStyle::Normal;

        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;

        float paddingTop = 0, paddingBottom = 0, paddingLeft = 0, paddingRight = 0;

        float marginTop = 0, marginBottom = 0, marginLeft = 0, marginRight = 0;

        float borderTopWidth = 0, borderBottomWidth = 0, borderLeftWidth = 0, borderRightWidth = 0;
        RayColor borderColor = BLACK;
        float borderTopLeftRadius = 0, borderTopRightRadius = 0, borderBottomLeftRadius = 0, borderBottomRightRadius = 0;

        FlexDirection flexDir = FlexDirection::Column;
        AlignType justifyContent = AlignType::FlexStart;
        AlignType alignItems = AlignType::Start;

        OverflowMode overflow = OverflowMode::Visible;
        PositionType position = PositionType::Auto;

        int zIndex = 0;
        optional<RectF> clipRect = std::nullopt;

        RayColor color = BLACK;
        RayColor backgroundColor = RayColor { 0, 0, 0, 0 };

        style_t style;

        // TODO: maybe figure out how we can restore cache friendliness herein ! :)
        std::list<UIElement> children = {}; // list is necessary (otherwise need unique/shared ptr) since elements are not moved (ruins elementLookup)
        UIElement* parent = nullptr;

        optional<vector<std::function<void(UIElement&)>>> additionalRenderCalls = std::nullopt;

    private:
        // non-user-facing? TODO: organize these please
        void ApplyStyle();

        void JustifyChildren();
        void AlignChildren();
        void ShiftRelativeChildren();

        void SizeSelfByContent();
        void ApplyAspectRatio();
        void ApplyChildrenAspectRatios();

        void RelativelySizeChildren();
        void RelativelySizeFractionalChildren();
        void RelativelySizePercentageChildren();

        [[nodiscard]] bool WidthDir() const { return flexDir == FlexDirection::Row ? PRIMARY : SECONDARY; }
        [[nodiscard]] bool HeightDir() const { return !WidthDir(); }

        [[nodiscard]] bool HasPersonalClipping() const { return overflow == OverflowMode::Hidden || overflow == OverflowMode::Scroll; }
        [[nodiscard]] bool HasBorderRounding() const;
        [[nodiscard]] bool HasBorder() const;
        [[nodiscard]] bool HasBackground() const;
        [[nodiscard]] bool HasTexture() const;
        [[nodiscard]] bool HasText() const;
        [[nodiscard]] bool HasCustomRenderFn() const;
        [[nodiscard]] bool HasRenderables() const;
        [[nodiscard]] bool HasVisibleClipRect() const;

        void BeginClipRect();
        void EndClipRect();

        // >> rendering
        void RenderBackground() const;
        void RenderBorder() const;
        void RenderTexture();
        void RenderText() const;

        // >> false for those w/ position: { Fixed, Absolute, etc... }
        [[nodiscard]] bool IsContentAttached() const;

        auto ContentAttachedChildren() {
            return children | filter([](UIElement& child) { return child.IsContentAttached(); }) | collect_refs();
        }

        optional<RectF> GenOwnClipRect();
        static optional<RectF> IntersectionRects(const optional<RectF>& rect1, const optional<RectF>& rect2);

#define DEFINE_DIRECTIONAL_SIZE_GET(name, l, r) float& name(bool dir, UIElement& el) const { return ((flexDir == FlexDirection::Row) ^ dir) ? el.l : el.r; } float& name(bool dir) { return name(dir, *this); }
        // NOTE: Dimen(dir, child) != child.Dimen(dir) `flexDir context changes!`
        DEFINE_DIRECTIONAL_SIZE_GET(Dimen, width, height);
        DEFINE_DIRECTIONAL_SIZE_GET(PaddingFirst, paddingLeft, paddingTop);
        DEFINE_DIRECTIONAL_SIZE_GET(PaddingLast, paddingRight, paddingBottom);
        DEFINE_DIRECTIONAL_SIZE_GET(MarginFirst, marginLeft, marginTop);
        DEFINE_DIRECTIONAL_SIZE_GET(MarginLast, marginRight, marginBottom);
        DEFINE_DIRECTIONAL_SIZE_GET(BorderWidthFirst, borderLeftWidth, borderTopWidth);
        DEFINE_DIRECTIONAL_SIZE_GET(BorderWidthLast, borderRightWidth, borderBottomWidth);
        DEFINE_DIRECTIONAL_SIZE_GET(Coord, x, y);

        float TotalDimen(bool dir, UIElement& el) const {
            return BorderWidthFirst(dir, el)
                + MarginFirst(dir, el)
                + PaddingFirst(dir, el)
                + Dimen(dir, el)
                + PaddingLast(dir, el)
                + MarginLast(dir, el)
                + BorderWidthLast(dir, el);
        }
        float TotalDimen(bool dir) { return TotalDimen(dir, *this); }

#define DEFINE_DIRECTIONAL_SIZE_STYLE_GET(name, l, r) optional<SizeVariant>& name(bool dir, UIElement& el) const { return ((flexDir == FlexDirection::Row) ^ dir) ? el.l : el.r; } optional<SizeVariant>& name(bool dir) { return name(dir, *this); }

        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(DimenStyle, style.width, style.height);
        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(PaddingFirstStyle, style.paddingLeft, style.paddingTop);
        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(PaddingLastStyle, style.marginRight, style.marginBottom);
        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(MarginFirstStyle, style.marginLeft, style.marginTop);
        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(MarginLastStyle, style.marginRight, style.marginBottom);
        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(BorderWidthFirstStyle, style.borderLeftWidth, style.borderTopWidth);
        DEFINE_DIRECTIONAL_SIZE_STYLE_GET(BorderWidthLastStyle, style.borderRightWidth, style.borderBottomWidth);
    };
}
