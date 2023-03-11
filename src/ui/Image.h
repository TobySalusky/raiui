//
// Created by Tobiathan on 3/11/23.
//

#ifndef ARTNET_IMAGE_H
#define ARTNET_IMAGE_H

#include "Includes.h"
#include "Style.h"
#include "Identification.h"

namespace tui {
    void Image(const string& texture, const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current());
}

#endif //ARTNET_IMAGE_H
