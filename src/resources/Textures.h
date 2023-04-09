//
// Created by Tobiathan on 11/19/22.
//

#pragma once
#include "Includes.h"

class Textures {
public:
    static void Load();
    static RayTexture& Get(const string& textureName);

private:
    static inline unordered_map<string, RayTexture> textureMap = {};
};
