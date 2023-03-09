//
// Created by Tobiathan on 1/15/23.
//

#ifndef ARTNET_SLOT_H
#define ARTNET_SLOT_H


#include "UIElement.h"

namespace tui {

    struct Slot {

        void Here();
        ~Slot();

    private:
        UIElement* hereTop = nullptr;
    };
}


#endif //ARTNET_SLOT_H
