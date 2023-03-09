//
// Created by Tobiathan on 1/15/23.
//

#include "Slot.h"
#include "DOM.h"

void tui::Slot::Here() {
    if (hereTop != nullptr) {
        throw std::runtime_error("slot.Here() called more than once on a slot during one frame!");
    }

    hereTop = DOM::Current().GetCurrTop();

    if (hereTop == nullptr) {
        throw std::runtime_error("slot.Here() called on null parent!");
    }
}

tui::Slot::~Slot() {
    if (hereTop == nullptr) {
        Log("[WARNING]: slot.Here() was never called");
    }

    DOM::Current().AddBackToTop(hereTop);
}
