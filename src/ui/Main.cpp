#include "Main.h"
#include "DOM.h"
#include "Styles.h"

void tui::BeginUI() {
    // TODO: perhaps move this??
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DOM::UnFocus();
    }
}

void tui::EndUI() {
    // TODO: do nothing????
}

void tui::RenderUI() {
    DOM::Current().Build();
    DOM::Current().Render();

    // prepares for next frame
    DOM::SwapBuffers();
}

void tui::Initialize(const TuiInitializationSettings& settings) {
    Main::Initialize(settings);

    Styles::Initialize();

    if (settings.cssDirectoryPath) {
        Styles::LoadCssDirectory(*settings.cssDirectoryPath);
    }

    DOM::Initialize();
//    AppState::Initialize();
}

void tui::Main::Initialize(const tui::TuiInitializationSettings& settings) {
    instance = std::make_unique<Main>(settings);
}

tui::Main::Main(const tui::TuiInitializationSettings& settings) {
    textureLoadCallback = settings.textureLoadCallback;
}

RayTexture *tui::Main::LookupTextureFromString(const string& textureName) {
    return Instance().textureLoadCallback(textureName);
}

tui::Main& tui::Main::Instance() {
    return *instance;
}
