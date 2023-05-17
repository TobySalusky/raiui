//
// Created by Tobiathan on 5/16/23.
//

#pragma once
#include <functional>
#include "Identification.h"

namespace tui {
    struct size_routine {
        size_routine() = delete;
        int id;
        [[nodiscard]] float Invoke(float on) const;
        void Publish(const std::function<float(float)>& fn) const;

    private:
        friend class SizeRoutines;
        explicit size_routine(int id) : id(id) {}
    };

    class SizeRoutines {
    public:
        static size_routine Establish();
        static float Invoke(const size_routine& routine, float on) {
            return (routineFns.at(routine.id))(on);
        }
        static void Publish(const size_routine& routine, const std::function<float(float)>& fn);

    private:
        static inline std::vector<std::function<float(float)>> routineFns = {};
    };

    size_routine& UseSizeRoutine(const std::function<float(float)>& fn, const id_t& idLike = "", tloc location = tloc::current());
}