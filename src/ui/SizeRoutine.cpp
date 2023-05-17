//
// Created by Tobiathan on 5/16/23.
//
#include "SizeRoutine.h"
#include "State.h"

float tui::size_routine::Invoke(float on) const {
    return SizeRoutines::Invoke(*this, on);
}

void tui::size_routine::Publish(const std::function<float(float)>& fn) const {
    SizeRoutines::Publish(*this, fn);
}

tui::size_routine tui::SizeRoutines::Establish() {
    SizeRoutines::routineFns.emplace_back();
    return tui::size_routine((int) SizeRoutines::routineFns.size() - 1);
}

void tui::SizeRoutines::Publish(const tui::size_routine& routine, const std::function<float(float)>& fn) {
    if (routine.id < 0 || routine.id >= routineFns.size()) {
        throw std::runtime_error("size_routine id out of range!");
    }
    routineFns[routine.id] = fn;
}

tui::size_routine& tui::UseSizeRoutine(const std::function<float(float)>& fn, const id_t& idLike, tloc location) {
    size_routine& sr = UseInitRef([](){ return SizeRoutines::Establish(); }, idLike, location);
    sr.Publish(fn);
    return sr;
}
