//
// Created by Tobiathan on 12/26/22.
//

#include "DOM.h"
#include <stdexcept>

void tui::DOM::SetUp() {
    if (!scopeIdExtensionStack.empty()) {
        Log("[WARNING]: finished frame (re-init) with scopeIdExtensionStack non-empty top-value=`{}`", GetScopeId());
    }

    elementLookup.clear();
    renderGraph.clear();
    elementIdCountMap.clear();
    attachStack = {};
    root = { .id = "__root__"s, .style = Style{ .width = 1920.0f / 2, .height = 1080.0f / 2, .color = WHITE } };
    attachStack.push(&(this->root));
    Register(this->root);
}

void tui::DOM::Attach(UIElement elem) {
    if (attachStack.empty()) {
        throw std::runtime_error("Called attach while missing root!");
    }

    elem.parent = attachStack.top();
    attachStack.top()->children.push_back(elem);
    attachStack.push(&(attachStack.top()->children.back()));
    Register(elem);
}

void tui::DOM::Register(const UIElement& elem) {
    if (elem.id.empty()) { return; } // NOTE: empty id not used

    if (elementLookup.contains(elem.id)) {
        Log("[WARNING]: duplicated element with id `{}`!", elem.id);
    }
    elementLookup[elem.id] = attachStack.top();
}

void tui::DOM::Pop() {
    if (attachStack.empty()) {
        throw std::runtime_error("Popped from DOM too many times!");
    }

    attachStack.pop();
}

tui::DOM& tui::DOM::Current() {
    return (globals->dom1IsCurrent) ? globals->dom1 : globals->dom2;
}

tui::DOM& tui::DOM::Previous() {
    return (globals->dom1IsCurrent) ? globals->dom2 : globals->dom1;
}

void tui::DOM::SwapBuffers() {
    Previous().SetUp();
    globals->dom1IsCurrent ^= true;
}

string tui::DOM::Stringify() {
    return root.RecursiveStringify();
}

optional_ref<tui::UIElement> tui::DOM::Lookup(const string &id) {
    if (id.empty() || !elementLookup.contains(id)) {
        return std::nullopt;
    }
    return elementLookup[id];
}

bool tui::DOM::HasScopeId() {
    return !scopeIdExtensionStack.empty();
}

const string& tui::DOM::GetScopeId() {
    if (scopeIdExtensionStack.empty()) {
        throw std::runtime_error("GetScopeId called with empty scopeIdExtensionStack");
    }
    return scopeIdExtensionStack.top();
}

void tui::DOM::AddScopeId(const string& extension) {
    scopeIdExtensionStack.push(
            HasScopeId()
            ? scopeIdExtensionStack.top() + "-" + extension
            : extension
        );
}

void tui::DOM::PopScopeId() {
    if (scopeIdExtensionStack.empty()) {
        throw std::runtime_error("PopScopeId with empty scopeIdExtensionStack");
    }
    scopeIdExtensionStack.pop();
}

void tui::DOM::Initialize() {
    globals = std::make_unique<DOMGlobals>();
}

void tui::DOM::Build() {
    root.BuildUp();
    root.BuildDown();
    vector<UIElement*> elementTraversal = {};
    root.ClippingPreorderTraversal(elementTraversal);
    std::stable_sort(BEG_END(elementTraversal), [](UIElement* a, UIElement* b) {
        return a->zIndex < b->zIndex;
    });

    // filter out invisible elements
    elementTraversal = elementTraversal
            | FILTER_CALL(->IsVisible())
            | tutil::to_vector();

    // TODO: make func!! simplify!!
    optional<RectF> currClipRect = std::nullopt;

    renderGraph.reserve(elementTraversal.size());
    for (int i = 0; i < elementTraversal.size(); i++) {
        UIElement& elem = *elementTraversal[i];
        bool beginClip = false;
        bool endClip = false;

        if (!UIElement::RectanglesMatch(currClipRect, elem.clipRect)) {
            beginClip = elem.clipRect.has_value();
            currClipRect = elem.clipRect;
        }

        if (i == elementTraversal.size() - 1 || !UIElement::RectanglesMatch(elem.clipRect, elementTraversal[i + 1]->clipRect)) {
            endClip = elem.clipRect.has_value();
        }

        UIElement::RenderInfo info = { beginClip, endClip };
        renderGraph.emplace_back(&elem, info);
    }
}

void tui::DOM::Render() {
    for (auto& [elem, info] : renderGraph) {
        elem->Render(info);
    }
}

int tui::DOM::GetAndIncrementCountForId(const string& id) {
    return elementIdCountMap[id]++;
}

void tui::DOM::Focus(const string& id) {
    if (focusedId) { UnFocus(); }
    focusedId = id;
}

void tui::DOM::UnFocus() {
    if (focusedId) { prevFocusedId = focusedId; }
    focusedId = std::nullopt;
}

const optional<string>& tui::DOM::FocusedId() {
    return focusedId;
}
