//
// Created by Tobiathan on 3/11/23.
//

#pragma once

#include "Includes.h"
#include "Identification.h"
#include "Style.h"

namespace tui {
    void Leaf(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current());
}
