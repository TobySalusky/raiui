//
// Created by Tobiathan on 1/22/23.
//

#pragma once


#include "Includes.h"
#include "Style.h"

class ColorConverter {
public:
    static RayColor to_color(const ColorVariant& colorVariant);

private:
    static RayColor HexToRGB(string_view hexColor);
    static RayColor NamedColorToColor(const string& namedColor);
    static unordered_map<string, RayColor> namedColorLookup;
};
