//
// Created by Tobiathan on 12/26/22.
//

#ifndef ARTNET_MAIN_H
#define ARTNET_MAIN_H

#include "Includes.h"

using texture_load_callback_t = std::function<RayTexture*(const string&)>;

namespace tui {
    struct TuiInitializationSettings {
        optional<string> cssDirectoryPath;
        texture_load_callback_t textureLoadCallback = [](const string& textureName) -> RayTexture* {
            throw std::runtime_error("string-to-texture-lookup not implemented -- please specify textureLoadCallback setting with tui::Initialize");
        };
    };

    void Initialize(const TuiInitializationSettings& settings = {}); // load CSS
    void BeginUI();
    void EndUI();
    void RenderUI();

    class Main {
    public:
        static void Initialize(const TuiInitializationSettings& settings = {});

        explicit Main(const TuiInitializationSettings& settings);

        static RayTexture* LookupTextureFromString(const string& textureName);

    private:
        static Main& Instance(); // MUST BE INITIALIZED

        static inline unique_ptr<Main> instance = {};

        texture_load_callback_t textureLoadCallback;
    };
}

#endif //ARTNET_MAIN_H
