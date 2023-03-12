//
// Created by Tobiathan on 12/10/22.
//

#pragma once

#include "Includes.h"
#include "Paths.h"

enum class FontStyle {
    Normal, Bold, Italic
};

struct FontFamily {
    explicit FontFamily(const string& name) {
        normal = { Paths::Font(name + ".ttf") };
        bold = { Paths::Font(name + "Bold.ttf") };
        italic = { Paths::Font(name + "Italic.ttf") };
    }

    RayFont normal;
    RayFont bold;
    RayFont italic;
};

struct AvailableFonts {
    FontFamily primary = FontFamily("JetBrainsMono");
};

class Fonts {
public:
    static void Load() { fontHolder = std::make_unique<AvailableFonts>(); }

    static RayFont& Primary() { return fontHolder->primary.normal; }

    static RayFont& Get(FontStyle style = FontStyle::Normal) {
        if (style == FontStyle::Bold) { return fontHolder->primary.bold; }
        if (style == FontStyle::Italic) { return fontHolder->primary.italic; }
        return fontHolder->primary.normal;
    }

private:
    static inline unique_ptr<AvailableFonts> fontHolder = {};
};
