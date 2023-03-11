//
// Created by Tobiathan on 12/26/22.
//

#pragma once

#include <utility>
#include <stack>

#include "Includes.h"
#include "UIElement.h"

namespace tui {
    struct DOMGlobals;

    class DOM {
    public:
        static void Initialize(); // MUST INITIALIZE BEFORE USE

        static DOM& Current();
        static DOM& Previous();
        static void SwapBuffers();

        string Stringify();

        void Build();
        void Render();

        static void CloseScope() { Current().Pop(); }

        void Attach(UIElement elem); // TODO: MANY COPIES -- OPTIMIZE
        void Pop();
        void Register(const UIElement& elem);
        void PopAttach(UIElement elem) { Attach(std::move(elem)); Pop(); }

        optional<UIElement*> Lookup(const string& id);

        bool HasScopeId();
        const string& GetScopeId();
        void AddScopeId(const string& extension);
        void PopScopeId();

        [[nodiscard]] int GetAndIncrementCountForId(const string& id);

        DOM() { SetUp(); }

        // unsafe ----------------------------
        UIElement* GetCurrTop() { return attachStack.empty() ? nullptr : attachStack.top(); }
        void AddBackToTop(UIElement* elPtr) { attachStack.push(elPtr); }
    private:

        UIElement root = { .id = "invalid" };
        std::stack<UIElement*> attachStack = {};
        unordered_map<string, UIElement*> elementLookup = {};
        unordered_map<string, int> elementIdCountMap = {};
        std::stack<string> scopeIdExtensionStack = {};
        std::vector<std::pair<UIElement*, UIElement::RenderInfo>> renderGraph = {};

        void SetUp();

        static inline unique_ptr<DOMGlobals> globals = {};
    };

    struct DOMGlobals {
        DOM dom1 = DOM();
        DOM dom2 = DOM();
        bool dom1IsCurrent = true;
    };
}
