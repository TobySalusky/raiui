//
// Created by Tobiathan on 3/11/23.
//

#pragma once

#include "Includes.h"

class Canvas {
public:
    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;
    [[nodiscard]] const RayColor& GetBackgroundColor() const;

private:
    int width = 1920;
    int height = 1080;
    RayColor backgroundColor = WHITE;
};