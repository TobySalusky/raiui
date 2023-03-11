//
// Created by Tobiathan on 3/11/23.
//

#ifndef ARTNET_LEAF_H
#define ARTNET_LEAF_H

#include "Includes.h"
#include "Identification.h"
#include "Style.h"

namespace tui {
    void Leaf(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current());
}

#endif //ARTNET_LEAF_H
