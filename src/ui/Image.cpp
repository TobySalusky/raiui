//
// Created by Tobiathan on 3/11/23.
//

#include "Image.h"
#include "Leaf.h"

void tui::Image(const string& texture, const style_t &style, const id_t& idLike, tloc location) {
    Leaf(combined_styles(style, Style { .texture = texture }), idLike, location);
}