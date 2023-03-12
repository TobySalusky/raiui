//
// Created by Tobiathan on 3/11/23.
//

#pragma once

#include "Includes.h"

struct CanvasView {
    Vec2 offset = {0.0f, 0.0f};    // TODO: describe :)
    float scale = 1.0f;    // canvas will appear $scale \times dimens$ large
    float rotation = 0.0f; // radians
};
