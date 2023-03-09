//
// Created by Tobiathan on 11/19/22.
//

#ifndef MARIOGAME_TEXTURES_H
#define MARIOGAME_TEXTURES_H

#include "Includes.h"

class Textures {
public:
    static void Load();
    static RayTexture& Get(const string& textureName);

private:
    static inline unordered_map<string, RayTexture> textureMap = {};
};


#endif //MARIOGAME_TEXTURES_H
