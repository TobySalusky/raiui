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
void Program::Update(float deltaTime) {}

void Program::Render() {
    SCREEN_WIDTH = GetScreenWidth();
    SCREEN_HEIGHT = GetScreenHeight();

//    double startUI = GetTime();
    BeginUI();
    {
        RayColor panelColor = { 50, 50, 50 };

        Div page (Style { .dimens = {{ SCREEN_WIDTH, SCREEN_HEIGHT }} });
        FlexVert screenContainer ("fill");
        {
            FlexHoriz panelContainer("fill");
            {
                Panel panel({ .defaultWidth = 200.0f, .minWidth = 50.0f},
                            Style{ .backgroundColor = panelColor });
                FilePicker filePicker("/Users/toby/CLionProjects/TestModule");
            }

            {
                Panel panel2({ .dir = PanelDirection::Left, .defaultWidth = 200.0f, .minWidth = 150.0f },
                             Style{ .align = AlignType::Center, .backgroundColor = panelColor });
                {
                    LayerUI();
                }
            }
        }
        {
            Span bottomBar ("bottom-bar");
            Text("TODO...", Style{ .fontSize = 20, .padding = 5, .color = GRAY });
            Text("FPS: "s + std::to_string(GetFPS()), Style{ .fontSize = 20, .padding = 5, .color = GRAY });
        }
    }
    EndUI();
//    Log("UI took: {}", GetTime() - startUI);

    BeginDrawing();
    ClearBackground(RAYWHITE);

//    double startRender = GetTime();
    RenderUI();
//    Log("Render took: {}", GetTime() - startRender);

    EndDrawing();
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
            Interactive layerBox(InteractiveClass{"layer-box"});
            {
                {
                    Button toggle(content{}, "layer-toggle-box center");
                    Div eye(layer.visible ? "layer-eye-open" : "layer-eye-close");

                    if (auto tp = DelayTooltip(toggle)) {
                        Text("Toggle Visibility", Style{ .fontSize = 16, .padding = 5, .borderRadius = 3, .color = "ghostwhite", .backgroundColor = DARKGRAY, .overflow = OverflowMode::Hidden });
                    }

                    if (toggle.Pressed()) { layer.visible ^= true; }
                }
                {
                    Div img(ClassAndStyle{ "layer-image", { .texture = layer.textureName }});
                }
                Text(layer.name, "layer-text");
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
                Text(tooltip, Style{ .fontSize = 16, .padding = 5, .borderRadius = 3, .color = "ghostwhite", .backgroundColor = DARKGRAY });
            }
        };

        LayerButton("+", "Add layer", [&](){
            layers.push_back({"Colour"s + std::to_string(layers.size()), "kitasad" });
        });

        LayerButton("-", "Delete layer", [&](){
            if (!layers.empty()) { layers.pop_back(); }
        });
    }
}

#pragma clang diagnostic pop