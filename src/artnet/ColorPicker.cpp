//
// Created by Tobiathan on 5/13/23.
//

#include "ColorPicker.h"
#include "../ui/TextInput.h"
#include <UICommon.h>

using namespace tui;

void Block(RayColor& color) {
    const float HUE_SLIDER_WIDTH = 15.0f;

    Span blockRow("fill");
    Leaf(CStyle{
        "color-picker-current-display-block",
        { .backgroundColor = color }
    });
    Interactive blockCircle ("color-picker-circle center");
    Interactive blockInteractor (Style {
            .dimen = 50_pct, // TODO: need actual!
            .renderFn = [=](const RenderParams& params) {
                params.elem.GetBorderedBounds().DrawGradient(
                        RayColor::White(),
                        RayColor::Black(),
                        RayColor::Black(),
                        RayColor::FromHSV(
                                color.ToHSV().x,
                                1.0f,
                                1.0f
                        )
                );
            }
    });

    raylib::Shader s("", "");
    
    if (blockInteractor.Down()) {
        auto [saturation, valueInv] = blockInteractor.DownAtBoundedNorm();
        color = ColorFromHSV(color.ToHSV().x, saturation, 1.0f - valueInv);

        // TODO: hue
    }
}

void ReadOut(RayColor& color) {
    string& readout = UseRef(""s);

    Span rgbReadoutLine ("fill-width");
    TextInput readoutInput (readout, "color-picker-readout-input");
    if (!readoutInput.IsFocused()) {
        readout = std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b);
    }
    // TODO: input functionality!

    Interactive copy ("w:20px fill-height center", InteractiveStyles{
        .hover = "color-picker-readout-copy-btn-hover"
    });
    ImageLeaf ("copy", "dim:16px");

    if (copy.Pressed()) { SetClipboardText(readout.c_str()); }
}

void art_net::ColorPicker(RayColor& color, const tuid_t& idLike, tloc location) {
    ScopeId pickerId (gen_id(idLike, location));
    Div container ("color-picker-container");
    Block(color);
    ReadOut(color);
}
