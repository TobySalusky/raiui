//
// Created by Tobiathan on 12/10/22.
//

#pragma once

// GLOBAL INCLUDES -- these are all accessible by with just `#include "Includes.h"`
#include "raylib-cpp.hpp"
#include <vector>
#include <string>
#include <memory>
#include <ranges>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <numeric>

#include "../src/util/Logger.h"
#include "../src/util/Util.h"
#include "../src/ui/tloc.h"
#include "../src/util/optional_ref.h"

// GLOBAL typedefs/aliases -- shorten lengthy type names for more ergonomic coding :)

// raylib ========
using Vec2 = raylib::Vector2;
using Vec3 = raylib::Vector3;
using Vec4 = raylib::Vector4;

using RectF = raylib::Rectangle;
using RayTexture = raylib::Texture;
using RayColor = raylib::Color;
using RayFont = raylib::Font;

// GLOBAL using namespaces -- auto-include certain parts of std
using namespace std::views;
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

template <typename T>
using vector = std::vector<T>;

template <typename T, typename K>
using unordered_map = std::unordered_map<T, K>;

template <typename T>
using unordered_set = std::unordered_set<T>;

using string = std::string;
using string_view = std::string_view;

template <typename T>
using unique_ptr = std::unique_ptr<T>;

template <typename T>
using shared_ptr = std::shared_ptr<T>;

template <typename T>
using optional = std::optional<T>;

template <typename... Args>
using variant = std::variant<Args...>;

template <typename... Args>
using tuple = std::tuple<Args...>;

#define BEG_END(v) v.begin(), v.end()


// credit: https://stackoverflow.com/questions/1082192/how-to-generate-random-variable-names-in-c-using-macros
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

// NOTE: UNIQUE_NAME uses __COUNTER__
#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)

#define MAP(a) transform([&](auto it) a)
#define MAP_V(a) transform([&](auto it) a) | to_vector()


#define MAP_CALL(a) transform([&](auto it) { return it a; })
#define MAP_FUNC(a) transform([&](auto it) { return a(it); })
#define FILTER_CALL(a) filter([&](auto it) { return it a; })
#define FILTER_OUT_CALL(a) filter([&](auto it) { return !it a; })
