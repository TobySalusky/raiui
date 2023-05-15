#pragma once
#include "raylib-cpp.hpp"

class shader_hue_circle_t {
public:
shader_hue_circle_t() : shader(0, "/Users/toby/Documents/GitHub/Rauii/assets/shaders/hue_circle.frag") {
uniLoc_texture0 = shader.GetLocation("texture0");
uniLoc_colDiffuse = shader.GetLocation("colDiffuse");
uniLoc_pixelWidth = shader.GetLocation("pixelWidth");
uniLoc_pixelHeight = shader.GetLocation("pixelHeight");
}

void set_texture0(const raylib::Texture& value) { shader.SetValue(uniLoc_texture0, value);}
void set_colDiffuse(raylib::Vector4 value) { shader.SetValue(uniLoc_colDiffuse, (void*) &value, SHADER_UNIFORM_VEC4);}
void set_pixelWidth(float value) { shader.SetValue(uniLoc_pixelWidth, (void*) &value, SHADER_UNIFORM_FLOAT);}
void set_pixelHeight(float value) { shader.SetValue(uniLoc_pixelHeight, (void*) &value, SHADER_UNIFORM_FLOAT);}

private:
raylib::Shader shader;

int uniLoc_texture0;
int uniLoc_colDiffuse;
int uniLoc_pixelWidth;
int uniLoc_pixelHeight;
};

class shader_color_picker_t {
public:
shader_color_picker_t() : shader(0, "/Users/toby/Documents/GitHub/Rauii/assets/shaders/color_picker.frag") {
uniLoc_hue = shader.GetLocation("hue");
}

void set_hue(float value) { shader.SetValue(uniLoc_hue, (void*) &value, SHADER_UNIFORM_FLOAT);}

private:
raylib::Shader shader;

int uniLoc_hue;
};