//
// Created by Tobiathan on 11/19/22.
//

#include "Textures.h"
#include "Paths.h"


void Textures::Load() {
    vector<string> imagePaths = raylib::LoadDirectoryFiles(Paths::Asset("images"));

    for (const string& path : imagePaths) {
        textureMap[raylib::GetFileNameWithoutExt(path)] = { path };
    }
}

RayTexture &Textures::Get(const string& textureName) {
    if (!textureMap.contains(textureName)) {
        throw std::invalid_argument("textureName: `"s + textureName + "` does not exist.");
    }
    return textureMap[textureName];
}
