//
// Created by Tobiathan on 2/24/23.
//

#pragma once

#include "Includes.h"
#include "UIComponent.h"

namespace tui {
    struct FilePicker : public UIComponent {
        explicit FilePicker(const string& topLevelDirPath, const id_t& idLike = "", tloc location = tloc::current());
    };
}
