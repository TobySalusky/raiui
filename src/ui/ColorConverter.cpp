//
// Created by Tobiathan on 1/22/23.
//

#include "ColorConverter.h"

RayColor ColorConverter::to_color(const ColorVariant& colorVariant) {
    using namespace tutil;
    if (auto colorStr = pick<string>(colorVariant)) {
        if (colorStr->starts_with('#')) { return HexToRGB(*colorStr); }
        return NamedColorToColor(*colorStr);
    }
    return std::get<RayColor>(colorVariant);
}

RayColor ColorConverter::NamedColorToColor(const string& namedColor) {
    if (!namedColorLookup.contains(namedColor)) {
        Log("[STYLE-WARNING]: `{}` is not a valid named-color!", namedColor);
        return PURPLE;
    }

    return namedColorLookup[namedColor];
}

RayColor ColorConverter::HexToRGB(string_view hexColor) {
    string_view sv = hexColor;

    const auto ValidateChars = [&](){
        if (hexColor[0] != '#') { return false; }
        for (int i = 1; i < hexColor.length(); i++) {
            char c = hexColor[i];
            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) { continue; }
            return false;
        }
        return true;
    };

    if (!ValidateChars() || (hexColor.length() != 7 && hexColor.length() != 9)) {
        Log("[STYLE-WARNING]: invalid hex-color `{}`", hexColor);
        return PURPLE;
    }

    const auto HexBibbitsToIntegralNumber = [](string_view hexSubStr){
        const auto HexBibbitinoToSingularIntegralNumeral = [](char c) {
            if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
            if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
            return c - '0';
        };

        return (HexBibbitinoToSingularIntegralNumeral(hexSubStr[0]) << 4)
              | HexBibbitinoToSingularIntegralNumeral(hexSubStr[1]);
    };

    return RayColor (
        HexBibbitsToIntegralNumber(hexColor.substr(1, 2)),
        HexBibbitsToIntegralNumber(hexColor.substr(3, 2)),
        HexBibbitsToIntegralNumber(hexColor.substr(5, 2)),
        (hexColor.size() == 9) ? HexBibbitsToIntegralNumber(hexColor.substr(7, 2)) : 255
        );
}


unordered_map<string, RayColor> ColorConverter::namedColorLookup = {
        {"black", RayColor {0,0,0} },
        {"silver", RayColor {192,192,192} },
        {"gray", RayColor {128,128,128} },
        {"white", RayColor {255,255,255} },
        {"maroon", RayColor {128,0,0} },
        {"red", RayColor {255,0,0} },
        {"purple", RayColor {128,0,128} },
        {"fuchsia", RayColor {255,0,255} },
        {"green", RayColor {0,128,0} },
        {"lime", RayColor {0,255,0} },
        {"olive", RayColor {128,128,0} },
        {"yellow", RayColor {255,255,0} },
        {"navy", RayColor {0,0,128} },
        {"blue", RayColor {0,0,255} },
        {"teal", RayColor {0,128,128} },
        {"aqua", RayColor {0,255,255} },
        {"aliceblue", RayColor {240,248,255} },
        {"antiquewhite", RayColor {250,235,215} },
        {"aqua", RayColor {0,255,255} },
        {"aquamarine", RayColor {127,255,212} },
        {"azure", RayColor {240,255,255} },
        {"beige", RayColor {245,245,220} },
        {"bisque", RayColor {255,228,196} },
        {"black", RayColor {0,0,0} },
        {"blanchedalmond", RayColor {255,235,205} },
        {"blue", RayColor {0,0,255} },
        {"blueviolet", RayColor {138,43,226} },
        {"brown", RayColor {165,42,42} },
        {"burlywood", RayColor {222,184,135} },
        {"cadetblue", RayColor {95,158,160} },
        {"chartreuse", RayColor {127,255,0} },
        {"chocolate", RayColor {210,105,30} },
        {"coral", RayColor {255,127,80} },
        {"cornflowerblue", RayColor {100,149,237} },
        {"cornsilk", RayColor {255,248,220} },
        {"crimson", RayColor {220,20,60} },
        {"cyan", RayColor {0,255,255} },
        {"darkblue", RayColor {0,0,139} },
        {"darkcyan", RayColor {0,139,139} },
        {"darkgoldenrod", RayColor {184,134,11} },
        {"darkgray", RayColor {169,169,169} },
        {"darkgreen", RayColor {0,100,0} },
        {"darkgrey", RayColor {169,169,169} },
        {"darkkhaki", RayColor {189,183,107} },
        {"darkmagenta", RayColor {139,0,139} },
        {"darkolivegreen", RayColor {85,107,47} },
        {"darkorange", RayColor {255,140,0} },
        {"darkorchid", RayColor {153,50,204} },
        {"darkred", RayColor {139,0,0} },
        {"darksalmon", RayColor {233,150,122} },
        {"darkseagreen", RayColor {143,188,143} },
        {"darkslateblue", RayColor {72,61,139} },
        {"darkslategray", RayColor {47,79,79} },
        {"darkslategrey", RayColor {47,79,79} },
        {"darkturquoise", RayColor {0,206,209} },
        {"darkviolet", RayColor {148,0,211} },
        {"deeppink", RayColor {255,20,147} },
        {"deepskyblue", RayColor {0,191,255} },
        {"dimgray", RayColor {105,105,105} },
        {"dimgrey", RayColor {105,105,105} },
        {"dodgerblue", RayColor {30,144,255} },
        {"firebrick", RayColor {178,34,34} },
        {"floralwhite", RayColor {255,250,240} },
        {"forestgreen", RayColor {34,139,34} },
        {"fuchsia", RayColor {255,0,255} },
        {"gainsboro", RayColor {220,220,220} },
        {"ghostwhite", RayColor {248,248,255} },
        {"gold", RayColor {255,215,0} },
        {"goldenrod", RayColor {218,165,32} },
        {"gray", RayColor {128,128,128} },
        {"green", RayColor {0,128,0} },
        {"greenyellow", RayColor {173,255,47} },
        {"grey", RayColor {128,128,128} },
        {"honeydew", RayColor {240,255,240} },
        {"hotpink", RayColor {255,105,180} },
        {"indianred", RayColor {205,92,92} },
        {"indigo", RayColor {75,0,130} },
        {"ivory", RayColor {255,255,240} },
        {"khaki", RayColor {240,230,140} },
        {"lavender", RayColor {230,230,250} },
        {"lavenderblush", RayColor {255,240,245} },
        {"lawngreen", RayColor {124,252,0} },
        {"lemonchiffon", RayColor {255,250,205} },
        {"lightblue", RayColor {173,216,230} },
        {"lightcoral", RayColor {240,128,128} },
        {"lightcyan", RayColor {224,255,255} },
        {"lightgoldenrodyellow", RayColor {250,250,210} },
        {"lightgray", RayColor {211,211,211} },
        {"lightgreen", RayColor {144,238,144} },
        {"lightgrey", RayColor {211,211,211} },
        {"lightpink", RayColor {255,182,193} },
        {"lightsalmon", RayColor {255,160,122} },
        {"lightseagreen", RayColor {32,178,170} },
        {"lightskyblue", RayColor {135,206,250} },
        {"lightslategray", RayColor {119,136,153} },
        {"lightslategrey", RayColor {119,136,153} },
        {"lightsteelblue", RayColor {176,196,222} },
        {"lightyellow", RayColor {255,255,224} },
        {"lime", RayColor {0,255,0} },
        {"limegreen", RayColor {50,205,50} },
        {"linen", RayColor {250,240,230} },
        {"magenta", RayColor {255,0,255} },
        {"maroon", RayColor {128,0,0} },
        {"mediumaquamarine", RayColor {102,205,170} },
        {"mediumblue", RayColor {0,0,205} },
        {"mediumorchid", RayColor {186,85,211} },
        {"mediumpurple", RayColor {147,112,219} },
        {"mediumseagreen", RayColor {60,179,113} },
        {"mediumslateblue", RayColor {123,104,238} },
        {"mediumspringgreen", RayColor {0,250,154} },
        {"mediumturquoise", RayColor {72,209,204} },
        {"mediumvioletred", RayColor {199,21,133} },
        {"midnightblue", RayColor {25,25,112} },
        {"mintcream", RayColor {245,255,250} },
        {"mistyrose", RayColor {255,228,225} },
        {"moccasin", RayColor {255,228,181} },
        {"navajowhite", RayColor {255,222,173} },
        {"navy", RayColor {0,0,128} },
        {"oldlace", RayColor {253,245,230} },
        {"olive", RayColor {128,128,0} },
        {"olivedrab", RayColor {107,142,35} },
        {"orange", RayColor {255,165,0} },
        {"orangered", RayColor {255,69,0} },
        {"orchid", RayColor {218,112,214} },
        {"palegoldenrod", RayColor {238,232,170} },
        {"palegreen", RayColor {152,251,152} },
        {"paleturquoise", RayColor {175,238,238} },
        {"palevioletred", RayColor {219,112,147} },
        {"papayawhip", RayColor {255,239,213} },
        {"peachpuff", RayColor {255,218,185} },
        {"peru", RayColor {205,133,63} },
        {"pink", RayColor {255,192,203} },
        {"plum", RayColor {221,160,221} },
        {"powderblue", RayColor {176,224,230} },
        {"purple", RayColor {128,0,128} },
        {"red", RayColor {255,0,0} },
        {"rosybrown", RayColor {188,143,143} },
        {"royalblue", RayColor {65,105,225} },
        {"saddlebrown", RayColor {139,69,19} },
        {"salmon", RayColor {250,128,114} },
        {"sandybrown", RayColor {244,164,96} },
        {"seagreen", RayColor {46,139,87} },
        {"seashell", RayColor {255,245,238} },
        {"sienna", RayColor {160,82,45} },
        {"silver", RayColor {192,192,192} },
        {"skyblue", RayColor {135,206,235} },
        {"slateblue", RayColor {106,90,205} },
        {"slategray", RayColor {112,128,144} },
        {"slategrey", RayColor {112,128,144} },
        {"snow", RayColor {255,250,250} },
        {"springgreen", RayColor {0,255,127} },
        {"steelblue", RayColor {70,130,180} },
        {"tan", RayColor {210,180,140} },
        {"teal", RayColor {0,128,128} },
        {"thistle", RayColor {216,191,216} },
        {"tomato", RayColor {255,99,71} },
        {"turquoise", RayColor {64,224,208} },
        {"violet", RayColor {238,130,238} },
        {"wheat", RayColor {245,222,179} },
        {"white", RayColor {255,255,255} },
        {"whitesmoke", RayColor {245,245,245} },
        {"yellow", RayColor {255,255,0} },
        {"yellowgreen", RayColor {154,205,50} }
};

/*
black 0,0,0
silver 192,192,192
gray 128,128,128
white 255,255,255
maroon 128,0,0
red 255,0,0
purple 128,0,128
fuchsia 255,0,255
green 0,128,0
lime 0,255,0
olive 128,128,0
yellow 255,255,0
navy 0,0,128
blue 0,0,255
teal 0,128,128
aqua 0,255,255
aliceblue 240,248,255
antiquewhite 250,235,215
aqua 0,255,255
aquamarine 127,255,212
azure 240,255,255
beige 245,245,220
bisque 255,228,196
black 0,0,0
blanchedalmond 255,235,205
blue 0,0,255
blueviolet 138,43,226
brown 165,42,42
burlywood 222,184,135
cadetblue 95,158,160
chartreuse 127,255,0
chocolate 210,105,30
coral 255,127,80
cornflowerblue 100,149,237
cornsilk 255,248,220
crimson 220,20,60
cyan 0,255,255
darkblue 0,0,139
darkcyan 0,139,139
darkgoldenrod 184,134,11
darkgray 169,169,169
darkgreen 0,100,0
darkgrey 169,169,169
darkkhaki 189,183,107
darkmagenta 139,0,139
darkolivegreen 85,107,47
darkorange 255,140,0
darkorchid 153,50,204
darkred 139,0,0
darksalmon 233,150,122
darkseagreen 143,188,143
darkslateblue 72,61,139
darkslategray 47,79,79
darkslategrey 47,79,79
darkturquoise 0,206,209
darkviolet 148,0,211
deeppink 255,20,147
deepskyblue 0,191,255
dimgray 105,105,105
dimgrey 105,105,105
dodgerblue 30,144,255
firebrick 178,34,34
floralwhite 255,250,240
forestgreen 34,139,34
fuchsia 255,0,255
gainsboro 220,220,220
ghostwhite 248,248,255
gold 255,215,0
goldenrod 218,165,32
gray 128,128,128
green 0,128,0
greenyellow 173,255,47
grey 128,128,128
honeydew 240,255,240
hotpink 255,105,180
indianred 205,92,92
indigo 75,0,130
ivory 255,255,240
khaki 240,230,140
lavender 230,230,250
lavenderblush 255,240,245
lawngreen 124,252,0
lemonchiffon 255,250,205
lightblue 173,216,230
lightcoral 240,128,128
lightcyan 224,255,255
lightgoldenrodyellow 250,250,210
lightgray 211,211,211
lightgreen 144,238,144
lightgrey 211,211,211
lightpink 255,182,193
lightsalmon 255,160,122
lightseagreen 32,178,170
lightskyblue 135,206,250
lightslategray 119,136,153
lightslategrey 119,136,153
lightsteelblue 176,196,222
lightyellow 255,255,224
lime 0,255,0
limegreen 50,205,50
linen 250,240,230
magenta 255,0,255
maroon 128,0,0
mediumaquamarine 102,205,170
mediumblue 0,0,205
mediumorchid 186,85,211
mediumpurple 147,112,219
mediumseagreen 60,179,113
mediumslateblue 123,104,238
mediumspringgreen 0,250,154
mediumturquoise 72,209,204
mediumvioletred 199,21,133
midnightblue 25,25,112
mintcream 245,255,250
mistyrose 255,228,225
moccasin 255,228,181
navajowhite 255,222,173
navy 0,0,128
oldlace 253,245,230
olive 128,128,0
olivedrab 107,142,35
orange 255,165,0
orangered 255,69,0
orchid 218,112,214
palegoldenrod 238,232,170
palegreen 152,251,152
paleturquoise 175,238,238
palevioletred 219,112,147
papayawhip 255,239,213
peachpuff 255,218,185
peru 205,133,63
pink 255,192,203
plum 221,160,221
powderblue 176,224,230
purple 128,0,128
red 255,0,0
rosybrown 188,143,143
royalblue 65,105,225
saddlebrown 139,69,19
salmon 250,128,114
sandybrown 244,164,96
seagreen 46,139,87
seashell 255,245,238
sienna 160,82,45
silver 192,192,192
skyblue 135,206,235
slateblue 106,90,205
slategray 112,128,144
slategrey 112,128,144
snow 255,250,250
springgreen 0,255,127
steelblue 70,130,180
tan 210,180,140
teal 0,128,128
thistle 216,191,216
tomato 255,99,71
turquoise 64,224,208
violet 238,130,238
wheat 245,222,179
white 255,255,255
whitesmoke 245,245,245
yellow 255,255,0
yellowgreen 154,205,50
*/
