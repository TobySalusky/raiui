//
// Created by Tobiathan on 1/10/23.
//

#ifndef ARTNET_FLEX_H
#define ARTNET_FLEX_H

#include "DOM.h"
#include "Text.h"
#include "Main.h"
#include "Style.h"
#include "Identification.h"

namespace tui {
    struct FlexVert : UIComponent {

        explicit FlexVert(const style_t &style = {}, const id_t& idLike = "",
                          tloc location = tloc::current())
                : UIComponent(idLike, location) {
            id = gen_id(idLike, location);
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                    .flexDir=FlexDirection::Column, .justifyContent = AlignType::SpaceBetween, .alignItems = AlignType::Center
                }, style)});
        }

        explicit FlexVert(AlignType justifyContent = AlignType::SpaceBetween, AlignType alignItems = AlignType::Center, const id_t& idLike = "", tloc location = tloc::current())
                          : FlexVert(Style {.justifyContent = justifyContent, .alignItems = alignItems}, idLike, location) {}

        ~FlexVert() {
            DOM::CloseScope();
        }
    };

    struct FlexHoriz : UIComponent {

        explicit FlexHoriz(const style_t &style = {}, const id_t& idLike = "", tloc location = tloc::current())
                : UIComponent(idLike, location){
            id = gen_id(idLike, location);
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                    .flexDir=FlexDirection::Row, .justifyContent = AlignType::SpaceBetween, .alignItems = AlignType::Center
            }, style)});
        }

        explicit FlexHoriz(AlignType justifyContent = AlignType::SpaceBetween, AlignType alignItems = AlignType::Center, const id_t& idLike = "", tloc location = tloc::current())
                : FlexHoriz(Style {.justifyContent = justifyContent, .alignItems = alignItems}, idLike, location) {}

        ~FlexHoriz() {
            DOM::CloseScope();
        }
    };

}
#endif //ARTNET_FLEX_H
