//
// Created by Tobiathan on 1/22/23.
//

#pragma once

#include "Includes.h"
#include "Style.h"

struct ClassStyle { string className; Style style; };

class CssParser {
public:
    static vector<ClassStyle> CssFileContentsToStyles(const string& contents);
    static Style CssBodyToStyle(string_view css);
private:
    static void ApplyCssLineToStyle(Style& style, string_view line);
};