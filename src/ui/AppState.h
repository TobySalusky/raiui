//
// Created by Tobiathan on 3/6/23.
//

#pragma once

#include "Includes.h"
#include <any>

namespace tui {
    class AppState {
    public:
        // TODO: please don't expose this!
        static unordered_map<string, std::any>& Globals() { return globals; }

    private:
        static inline unordered_map<string, std::any> globals = {};
    };
}