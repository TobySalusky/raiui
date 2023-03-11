//
// Created by Tobiathan on 3/11/23.
//

#pragma once

#include "Includes.h"
#include "Style.h"
#include "Identification.h"

namespace tui {
    void ImageLeaf(const string& texture, const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current());
}
