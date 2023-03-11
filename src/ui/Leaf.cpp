//
// Created by Tobiathan on 3/11/23.
//

#include "Leaf.h"
#include "DOM.h"

void tui::Leaf(const style_t& style, const tui::id_t& idLike, tloc location) {
    DOM::Current().PopAttach({.id = gen_id(idLike, location), .style=style});
}
