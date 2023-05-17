#pragma once
#include "raylib-cpp.hpp"

class shader_color_picker_t {
public:
shader_color_picker_t() : shader(0, "/Users/toby/Documents/GitHub/Rauii/assets/shaders/color_picker.frag") {
uniLoc_texture0 = shader.GetLocation("texture0");
uniLoc_colDiffuse = shader.GetLocation("colDiffuse");
uniLoc_hue = shader.GetLocation("hue");
}

void set_texture0(const raylib::Texture& value) { shader.SetValue(uniLoc_texture0, value);}
void set_colDiffuse(raylib::Vector4 value) { shader.SetValue(uniLoc_colDiffuse, (void*) &value, SHADER_UNIFORM_VEC4);}
void set_hue(float value) { shader.SetValue(uniLoc_hue, (void*) &value, SHADER_UNIFORM_FLOAT);}

operator ::Shader() { return shader; }

private:
raylib::Shader shader;

int uniLoc_texture0;
int uniLoc_colDiffuse;
int uniLoc_hue;
};

class shader_hue_ring_t {
public:
shader_hue_ring_t() : shader(0, "/Users/toby/Documents/GitHub/Rauii/assets/shaders/hue_ring.frag") {
uniLoc_texture0 = shader.GetLocation("texture0");
uniLoc_colDiffuse = shader.GetLocation("colDiffuse");
uniLoc_size = shader.GetLocation("size");
uniLoc_ring_width = shader.GetLocation("ring_width");
}

void set_texture0(const raylib::Texture& value) { shader.SetValue(uniLoc_texture0, value);}
void set_colDiffuse(raylib::Vector4 value) { shader.SetValue(uniLoc_colDiffuse, (void*) &value, SHADER_UNIFORM_VEC4);}
void set_size(float value) { shader.SetValue(uniLoc_size, (void*) &value, SHADER_UNIFORM_FLOAT);}
void set_ring_width(float value) { shader.SetValue(uniLoc_ring_width, (void*) &value, SHADER_UNIFORM_FLOAT);}

operator ::Shader() { return shader; }

private:
raylib::Shader shader;

int uniLoc_texture0;
int uniLoc_colDiffuse;
int uniLoc_size;
int uniLoc_ring_width;
};