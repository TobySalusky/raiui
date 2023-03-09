//
// Created by Tobiathan on 3/6/23.
//

#ifndef ARTNET_APPSTATE_H
#define ARTNET_APPSTATE_H

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



#endif //ARTNET_APPSTATE_H
