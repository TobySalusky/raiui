//
// Created by Tobiathan on 4/8/23.
//

#pragma once

template <typename T>
class optional_ref {
public:
    optional_ref(T* ptr) : opt(ptr == nullptr ? std::nullopt : std::optional<T*>{ ptr }) {}
    optional_ref(T& val) : opt(std::optional<T*>{ &val }) {}
    optional_ref(std::nullopt_t no) : opt(std::nullopt) {}

    constexpr T& value() {
        return *opt.value();
    }

    constexpr T& operator* () {
        return value();
    }

    constexpr T* operator-> () {
        return opt.value();
    }


    constexpr bool has_value() {
        return opt.has_value();
    }

    constexpr operator bool() {
        return has_value();
    }
private:
    std::optional<T*> opt;
};