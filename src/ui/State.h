//
// Created by Tobiathan on 1/14/23.
//

#ifndef ARTNET_STATE_H
#define ARTNET_STATE_H

#include <any>
#include "Includes.h"
#include "Identification.h"
#include "AppState.h"

namespace tui {
    template <typename T, typename R>
    concept functor_returning = requires(T val) {
        { val() } -> std::convertible_to<R>;
    };

    // TODO: make PersistState a func that just returns ref?? if that works...
    // NOTE: ^ is possible, at least for trivial :), maybe not setting though?
    template <typename T>
    struct PersistState {

        // TODO: add overload for trivial default_t, not functor, but actual value (better for simple types :O )
        template<typename state_default_functor_t> requires functor_returning<state_default_functor_t, T>
        explicit PersistState(state_default_functor_t defaultFunc, const id_t& idLike = "", tloc location = tloc::current()) {
            id = gen_id(idLike, location);

            auto& stateMap = AppState::Globals();
            if (!stateMap.contains(id)) {
                stateMap[id] = defaultFunc();
            }
        }

        T& operator*() {
            return get();
        }

        T& get() {
            return *std::any_cast<T>(&AppState::Globals()[id]);
        }

        void set(T&& newVal) {
            AppState::Globals()[id] = newVal;
        }

    private:
        string id;
    };

    // prefer this for trivial/cheap types -- no I/O or complex logic :)
    template<typename T>
    T& UseRef(T defaultValue, const id_t& idLike = "", tloc location = tloc::current()) {
        string id = gen_id(idLike, location);

        auto& stateMap = AppState::Globals();
        if (!stateMap.contains(id)) {
            stateMap[id] = defaultValue;
        }

        return *std::any_cast<T>(&AppState::Globals()[id]);
    }

    template<typename state_default_functor_t>
    auto& UseInitRef(state_default_functor_t defaultFunc, const id_t& idLike = "", tloc location = tloc::current()) {
        string id = gen_id(idLike, location);

        auto& stateMap = AppState::Globals();
        if (!stateMap.contains(id)) {
            stateMap[id] = defaultFunc();
        }

        return *std::any_cast<decltype(defaultFunc())>(&AppState::Globals()[id]);
    }
}


#endif //ARTNET_STATE_H
