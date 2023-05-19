//
// Created by Tobiathan on 5/13/23.
//

#include "ColorPicker.h"
#include "../ui/TextInput.h"
#include "compile/shaders.h"
#include "../resources/Textures.h"
#include "../ui/ZeroPoint.h"
#include "../ui/Alert.h"
#include <UICommon.h>

using namespace tui;

struct ColorPickerState {
    float hue;
    float saturation;
    float value;
};

void ColorCircle(RayColor color, Vec2 offset, tloc loc = tloc::current()) {
    ScopeId id(gen_id(loc));

    Div border (CStyle{"color-picker-color-circle-border", {
        .left = offset.x,
        .top = offset.y,
    }});
    Div outer ("color-picker-color-circle-outer");
    Div inner (CStyle{
        "color-picker-color-circle-inner",
        { .backgroundColor = color }
    });
}

void Block(ColorPickerState& my, RayColor color) {
    const float HUE_SLIDER_WIDTH = 20.0f;

    size_routine& blockSizer = UseSizeRoutine([=](float parent) {
        return std::sqrt((std::pow((parent / 2.0f) - HUE_SLIDER_WIDTH, 2.0f)) / 2.0f) * 2.0f;
    });

    const auto GetHuePositioner = [&](float(*mathematicalFn)(float), tloc loc = tloc::current()) {
        return UseSizeRoutine([=](float parent) {
            float dotRad = (parent / 2.0f) - (HUE_SLIDER_WIDTH / 2.0f);
            return mathematicalFn(my.hue * DEG2RAD) * dotRad + (parent / 2.0f);
        }, "", loc);
    };

    static shader_color_picker_t pickerShader;
    static shader_hue_ring_t hueShader;
    pickerShader.set_hue(my.hue / 360.0f);

    Span blockRow("fill");
    Leaf(CStyle{
        "color-picker-current-display-block",
        { .backgroundColor = color }
    });
    Interactive hueRing (CStyle{ "color-picker-circle", {
            .renderFn = [=](const RenderParams& params) {
                hueShader.set_ring_width(HUE_SLIDER_WIDTH);
                hueShader.set_size(params.elem.width);

                BeginShaderMode(hueShader);
                RayTexture& texture = Textures::Get("copy"); // TODO: remove, terrrible, bleh!
                texture.Draw(RectF { 0.0f, 0.0f, (float) texture.width, (float) texture.height }, params.elem.GetUnBorderedBounds());
                EndShaderMode();
            }
    }});
    {
        ZeroPoint zpHue (Style{
            .position = PositionType::Relative,
            .left = GetHuePositioner(std::cos),
            .top = GetHuePositioner(std::sin),
        });
        ColorCircle(ColorFromHSV(my.hue, 1.0f, 1.0f), { -7, -7 });
    }

    Div blockCenterer ("fill center");
    Interactive colorPickerBlock (Style {
            .dimen = blockSizer,
            .renderFn = [=](const RenderParams& params) {
                BeginShaderMode(pickerShader);
                RayTexture& texture = Textures::Get("copy"); // TODO: remove, terrrible, bleh!
                texture.Draw(RectF { 0.0f, 0.0f, (float) texture.width, (float) texture.height }, params.elem.GetUnBorderedBounds());
                EndShaderMode();
            }
    });
    ZeroPoint zpBlock (CStyle{"center", { .left = portion{ my.saturation }, .top = portion{ 1.0f - my.value } } });
    ColorCircle(color, { 0, -7 });

    bool& hueRingDown = UseRef(false);

    if (colorPickerBlock.Down()) {
        auto [newSaturation, valueInv] = colorPickerBlock.DownAtBoundedNorm();
        my.saturation = newSaturation;
        my.value = 1.0f - valueInv;

        // TODO: hue,s,v state!
    } else if (hueRing.Pressed() && hueRing.UsePrev()) {
        float hueRad = hueRing.UsePrev()->width / 2.0f;
        float hueDist = hueRing.DownAtCenterDist();

        hueRingDown = hueDist >= (hueRad - HUE_SLIDER_WIDTH) && hueDist <= hueRad;
    }

    if (!hueRing.Down()) {
        hueRingDown = false;
    }

    if (hueRingDown) {
        float hueAngle = hueRing.DownAtCenterAngle();

        my.hue = -hueAngle * RAD2DEG;
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

    TextAlert copiedAlert(copy, "copied!", "copy-alert-popup");

    if (copy.Pressed()) {
        SetClipboardText(readout.c_str());
        copiedAlert.Trigger();
    }
}

void BestMatchStateToReality(ColorPickerState& my, RayColor color) {
    if (RayColor(ColorFromHSV(my.hue, my.saturation, my.value)) == color) { return; }

    auto [newHue, newSaturation, newValue] = ColorToHSV(color);
    my.hue = newHue;
    my.saturation = newSaturation;
    my.value = newValue;
}

void ApplyStateChanges(ColorPickerState& my, RayColor& color) {
    color = ColorFromHSV(my.hue, my.saturation, my.value);
}
//#define ID_STUB_DEFN const tuid_t& idLike = "", tloc loc = tloc::current() TODO: ?? do i like these macros ?? :/
//#define ID_STUB_IMPL const tuid_t& idLike, tloc loc
//#define SCOPE_ID ScopeId _tui_scope_id__ (gen_id(idLike, loc))
void art_net::ColorPicker(RayColor& color, const tuid_t& idLike, tloc loc) {
    ScopeId pickerId (gen_id(idLike, loc));
    auto& my = UseRef<ColorPickerState>({});
    Div container ("color-picker-container");

    BestMatchStateToReality(my, color);
    Block(my, color);
    ApplyStateChanges(my, color);

    ReadOut(color);
}