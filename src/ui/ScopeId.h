//
// Created by Tobiathan on 1/9/23.
//

#ifndef ARTNET_SCOPEID_H
#define ARTNET_SCOPEID_H

#include "Includes.h"
#include "DOM.h"

namespace tui {
    struct ScopeId {
        explicit ScopeId(const string& prepend) {
            DOM::Current().AddScopeId(prepend);
        }

        ~ScopeId() {
            DOM::Current().PopScopeId();
        }
    };
}

//#define SCOPE_ID(prepend) ScopeId UNIQUE_NAME(tui__scope_id_) (prepend);


#endif //ARTNET_SCOPEID_H
