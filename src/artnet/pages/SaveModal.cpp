//
// Created by Tobiathan on 4/12/23.
//

#include "SaveModal.h"
#include "../../ui/TextInput.h"
#include "../../ui/FilePathInput.h"
#include <UICommon.h>

using namespace tui;

bool NeatButton(const string& text) {
    ScopeId id(text);

    Interactive button ("save-modal-button");

    Text(text, "save-modal-button-text");

    return button.Pressed();
}

void NeatTextBox() {
    string& it = UseRef(""s);

    FilePathInput(it, {
        .containerStyle = "modal-input-container",
        .inputStyle = "modal-input",
        .suggestionContainer = "modal-suggestion-container",
        .suggestionBox = "modal-suggestion-box",
        .suggestionBoxHover = "modal-suggestion-box-hover",
        .suggestionBoxActive = "modal-suggestion-box-active",
        .suggestTextMatch = "modal-file-path-input-text-match",
        .suggestTextNonMatch = "modal-file-path-input-text-non-match",
    });
}

void art_net::SaveModal() {
    Div screenDim ("modal-screen-dim center");
    Div container ("modal-container");
    {
        Div header ("fill-width center");
        Text("Save", "modal-title");
        Leaf("h-line bg:#757575");
    }
    {
        Div body ("fill center");
        NeatTextBox();
    }
    {
        Span buttonLine ("fill-width center");
        if (NeatButton("Cancel")) {

        }
        if (NeatButton("Export")) {

        }
    }
}