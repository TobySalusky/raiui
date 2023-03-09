//
// Created by Tobiathan on 1/22/23.
//

#ifndef ARTNET_CSSPARSER_H
#define ARTNET_CSSPARSER_H

#include "Includes.h"
#include "Style.h"

struct ClassStyle { string className; Style style; };

class CssParser {
public:
    static vector<ClassStyle> CssFileContentsToStyles(const string& contents);
private:
    static Style CssBodyToStyle(string_view css);
    static void ApplyCssLineToStyle(Style& style, string_view line);
};


#endif //ARTNET_CSSPARSER_H
