//
// Created by Tobiathan on 1/4/23.
//

#include "Identification.h"
#include "DOM.h"

string tui::gen_id(const string& id, tloc location) {
    return with_scope_id(id.empty() ? location_id(location) : id);
}

string tui::gen_id(const id_exact& idExact) {
    return idExact.id;
}

string tui::gen_id(const id_append& idExtension, tloc location) {
    return with_scope_id(location_id(location) + "-" + idExtension.extension);
}

string tui::gen_id(const id_count& idCounter, tloc location) {
    string idWithoutCount = with_scope_id(location_id(location));

    return with_scope_id(location_id(location) + "-" + std::to_string(DOM::Current().GetAndIncrementCountForId(idWithoutCount)));
}

string tui::gen_id(tloc location) {
    return gen_id(""s, location);
}

string tui::gen_id(const id_t& id, tloc location) {
    if (id.index() == std::variant_npos) {
        return gen_id(location);
    }

    if (auto idString = tutil::pick<string>(id)) {
        return gen_id(*idString, location);
    } else if (auto idExact = tutil::pick<id_exact>(id)) {
        return gen_id(*idExact);
    } else if (auto idCounter = tutil::pick<id_count>(id)) {
        return gen_id(*idCounter, location);
    } else if (auto idExtension = tutil::pick<id_append>(id)) {
        return gen_id(*idExtension, location);
    }
    throw std::runtime_error("unexpected type in id_t!");
}

tui::id_exact tui::predef(const id_t& id, tloc location) {
    return id_exact { gen_id(id, location) };
}


string tui::location_id(tloc location) {
    // NOTE: WARNING !! this may not work if multiple files have the same name!
    // TODO: cleanup, very ugly

    string fileName = location.file_name();
    size_t lastSlash = fileName.find_last_of('/');
    if (lastSlash == string::npos) {
        Log("uh oh {} :: {}", fileName, location.function_name());
        fileName = location.function_name();
        lastSlash = fileName.find_last_of('/');
        if (lastSlash == string::npos) {
            Log("bigger uh oh!");
            lastSlash = -1;
        }
    } // lol what

    return string("<") + fileName + "," + std::to_string(location.line()) + "," + std::to_string(location.column()) + ">";
}

string tui::with_scope_id(const string& id) {
    return DOM::Current().HasScopeId()
           ? DOM::Current().GetScopeId() + "-" + id
           : id;
}