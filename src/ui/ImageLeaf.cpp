//
// Created by Tobiathan on 3/11/23.
//

#include "ImageLeaf.h"
#include "Leaf.h"

void tui::ImageLeaf(const string& texture, const style_t &style, const id_t& idLike, tloc location) {
    // TODO: use hook function to get texture here for aspect ratio!!!
    Leaf(combined_styles(style, Style { .texture = texture }), idLike, location);
}