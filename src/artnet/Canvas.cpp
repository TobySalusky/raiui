//
// Created by Tobiathan on 3/11/23.
//

#include "Canvas.h"

int Canvas::GetWidth() const {
    return width;
}

int Canvas::GetHeight() const {
    return height;
}

const RayColor& Canvas::GetBackgroundColor() const {
    return backgroundColor;
}
