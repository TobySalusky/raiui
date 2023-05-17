#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//
// Created by Tobiathan on 12/10/22.
//


#include "Program.h"
#include "Includes.h"
#include "resources/Textures.h"
#include "resources/Fonts.h"
#include "ui/Main.h"
#include "ui/Button.h"
#include "ui/ScopeId.h"
#include "ui/Div.h"
#include "ui/Span.h"
#include "ui/Flex.h"
#include "ui/Panel.h"
#include "ui/Styles.h"
#include "ui/ScrollViewY.h"
#include "ui/Tooltip.h"
#include "ui/FilePicker.h"
#include "ui/OptStyles.h"
#include "ui/ImageLeaf.h"
#include "ui/TextInput.h"
#include "ui/KeyInput.h"
#include "artnet/pages/SaveModal.h"
#include "artnet/ColorPicker.h"

using namespace tui;
using namespace tutil;

void Program::Run() {
    // raylib
    raylib::Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Evil C++ HTML");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(600);
    SetConfigFlags(FLAG_MSAA_4X_HINT);      // Enable Multi Sampling Anti Aliasing 4x (if available)

//    SetWindowIcon(raylib::Image(Paths::Image("icon.png")));

    // mine
    Textures::Load();
    Fonts::Load();

    // tui
    tui::Initialize({
        .cssDirectoryPath = Paths::Asset("css"),
        .textureLoadCallback = [](const string& textureName) { return &Textures::Get(textureName); }
    });

    // HideCursor(); TODO: use custom cursor!

    while (!window.ShouldClose()) {
        Update(GetFrameTime());
        Render();
    }
}
void Program::Update(float deltaTime) {
    KeyInput::Collect();
}

void Program::Render() {
    SCREEN_WIDTH = GetScreenWidth();
    SCREEN_HEIGHT = GetScreenHeight();

    // ui
    BeginUI();
    UI();
    EndUI();

    // rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    RenderUI();

    EndDrawing();
}

void Program::UI() {
    Div page (Style { .dimens = {{ SCREEN_WIDTH, SCREEN_HEIGHT }} });
    FlexVert screenContainer ("fill");
    ContentStripUI();
    BottomBarUI();

    static bool inModal = false;
    if (KeyBinds::Pressed(KeyBind::TAB_FORWARDS)) { inModal ^= true; }
    if (inModal) {
        art_net::SaveModal();
    }
}

void Program::ContentStripUI() {
    FlexHoriz panelContainer("fill");
    LeftPanelUI();
    RightPanelUI();
}

void Program::BottomBarUI() {
    Span bottomBar ("bottom-bar");

    Vec2 mousePos = raylib::Mouse::GetPosition();
    Text("mouse: "s + std::to_string((int) mousePos.x) + ", " + std::to_string((int) mousePos.y), Style{ .fontSize = 20, .padding = 5, .color = GRAY });
    Text("FPS: "s + std::to_string(GetFPS()), Style{ .fontSize = 20, .padding = 5, .color = GRAY });
}

void Program::LeftPanelUI() {
    Panel panel({ .defaultWidth = 150.0f, .minWidth = 50.0f}, "primary-panel");
    {
        Div top("fill");
        static RayColor color = BLUE;
        art_net::ColorPicker(color);
    }
    {
        Div bottom("fill");
        FilePicker filePicker("/Users/toby/CLionProjects/TestModule");
    }
}

void Program::RightPanelUI() {
    Panel panel2({ .dir = PanelDirection::Left, .defaultWidth = 200.0f, .minWidth = 150.0f },
                 "primary-panel");
    LayerUI();
//    TextInput input;
}

void Program::LayerUI() {
    // TODO: make render culling IsVisible() actually work!! :)

    struct Layer {
        string name;
        string textureName;
        bool visible = true;
    };

    static vector<Layer> layers = {
            {"Line Art", "kitasad" },
            {"Colour", "kitahappy" },
            {"Colour2", "kitahappy" },
    };

    {
        ScrollViewY scrollableViewContainer ("layer-container");

        for (Layer& layer : layers) {
            ScopeId layerId (layer.name);
            Span layerBox("layer-box");
            {
                { // toggle
                    Interactive toggle(InteractiveClass { "layer-toggle-box" });
                    Div eye(layer.visible ? "layer-eye-open" : "layer-eye-close");

                    if (auto tp = DelayTooltip(toggle)) {
                        Text("Toggle Visibility", "dark-tooltip");
                    }

                    if (toggle.Pressed()) { layer.visible ^= true; }
                }
                { // stub
                    Interactive layerBoxStub(InteractiveClass {"layer-box-stub"});
                    ImageLeaf(layer.textureName, "layer-image");
                    Text(layer.name, "layer-text");
                }
            }
        }
    }
    {
        Span s("layer-buttons-container");
        const auto LayerButton = [](const string& text, const string& tooltip, auto onPress){
            ScopeId scopeId(text);

            Interactive button(InteractiveClass{"layer-button"});
            Text(text);
            if (button.Pressed()) {
                onPress();
            }
            if (auto tp = Tooltip(button)) {
                Text(tooltip, "dark-tooltip");
            }
        };

        LayerButton("+", "Add layer", [&](){
            layers.push_back({"Colour"s + std::to_string(layers.size()), "kitasad" });
        });

        LayerButton("-", "Delete layer", [&](){
            if (!layers.empty()) { layers.pop_back(); }
        });

        static string it = "hi";
        TextInput input(it);
    }
}

#pragma clang diagnostic pop