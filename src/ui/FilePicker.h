//
// Created by Tobiathan on 2/24/23.
//

#ifndef ARTNET_FILEPICKER_H
#define ARTNET_FILEPICKER_H

#include "Includes.h"
#include "UIComponent.h"

namespace tui {
    struct FilePicker : public UIComponent {
        explicit FilePicker(const string& topLevelDirPath, const id_t& idLike = "", tloc location = tloc::current());
    };
}


#endif //ARTNET_FILEPICKER_H
