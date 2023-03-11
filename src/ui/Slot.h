//
// Created by Tobiathan on 1/15/23.
//

#pragma once

#include "UIElement.h"

namespace tui {

    struct Slot {

        void Here();
        ~Slot();

    private:
        UIElement* hereTop = nullptr;
    };
}
