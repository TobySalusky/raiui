//
// Created by Tobiathan on 1/4/23.
//

#ifndef ARTNET_IDENTIFICATION_H
#define ARTNET_IDENTIFICATION_H

#include "Includes.h"
#include "Main.h"


namespace tui {
    struct id_exact {
        string id;
    };

    struct id_append {
        string extension;
    };

    struct id_count {};

    using id_t = variant<string, id_append, id_count, id_exact>;

    id_exact predef(const id_t& id = ""s, tloc location = tloc::current());

    string gen_id(const id_t& id, tloc location);
    string gen_id(const string& id, tloc location);
    string gen_id(const id_exact& id);
    string gen_id(const id_append& idExtension, tloc location);
    string gen_id(const id_count& idCounter, tloc location);
    string gen_id(tloc location = tloc::current());

    string location_id(tloc location);

    string with_scope_id(const string& id);
}

#endif //ARTNET_IDENTIFICATION_H
