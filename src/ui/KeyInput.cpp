//
// Created by Tobiathan on 3/20/23.
//

#include "KeyInput.h"

void KeyInput::Collect() {
    prevDown = down;

    // remove released keys
    vector<KeyboardKey> keysReleased = {};
    for (auto key : down) {
        if (IsKeyReleased(key)) { keysReleased.push_back(key); }
    }
    for (auto keyReleased : keysReleased) {
        down.erase(keyReleased); // TODO: fix! ?? KEY_LEFT_SUPER is released whenever KEY_BACKSPACE is pressed!?!?!
    }

    // add hit keys
    while (auto keyPressed = GetKeyPressed()) { // keyPressed == 0, when there are no more key presses
        down.insert((KeyboardKey) keyPressed);
    }

    // add ascii chars typed
    charsPressed.clear();
    while (auto charPressed = GetCharPressed()) { // charPressed == 0, when there are no more characters presses
        if ((charPressed >= 32) && (charPressed <= 125)) { // only accept ascii chars
            charsPressed.push_back((char) charPressed);
        }
    }
}
