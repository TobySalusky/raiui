//
// Created by Tobiathan on 1/10/23.
//

#pragma once

#include "DOM.h"
#include "Text.h"
#include "Main.h"
#include "Style.h"
#include "Identification.h"

namespace tui {
    struct FlexVert : OpenUIComponent {

        explicit FlexVert(const style_t &style = {}, const id_t& idLike = "",
                          tloc location = tloc::current())
                : OpenUIComponent(idLike, location) {
            id = gen_id(idLike, location);
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                    .flexDir=FlexDirection::Column, .justifyContent = AlignType::SpaceBetween, .alignItems = AlignType::Center
                }, style)});
        }

        explicit FlexVert(AlignType justifyContent = AlignType::SpaceBetween, AlignType alignItems = AlignType::Center, const id_t& idLike = "", tloc location = tloc::current())
                          : FlexVert(Style {.justifyContent = justifyContent, .alignItems = alignItems}, idLike, location) {}
    };

    struct FlexHoriz : OpenUIComponent {

        explicit FlexHoriz(const style_t &style = {}, AlignType justifyContent = AlignType::SpaceBetween, AlignType alignItems = AlignType::Center, const id_t& idLike = "", tloc location = tloc::current())
                : OpenUIComponent(idLike, location){
            id = gen_id(idLike, location);
            DOM::Current().Attach({.id = id, .style=combined_styles(Style {
                    .flexDir=FlexDirection::Row, .justifyContent = justifyContent, .alignItems = alignItems
            }, style)});
        }

        explicit FlexHoriz(AlignType justifyContent = AlignType::SpaceBetween, AlignType alignItems = AlignType::Center, const id_t& idLike = "", tloc location = tloc::current())
                : FlexHoriz({}, justifyContent, alignItems, idLike, location) {}
    };

}
