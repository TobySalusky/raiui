//
// Created by Tobiathan on 5/18/23.
//

#pragma once

#include "Canvas.h"
#include "CanvasView.h"

class Project {
public:
    Project(const Canvas& canvas) : canvas(canvas) {
        name = "Untitled";
        view = {};
    }

private:
    string name;

    Canvas canvas;
    CanvasView view;
};

class Projects {
public:
    optional_ref<Project> MaybeCurrent() {
        return current;
    }

    bool HasCurrent() {
        return current.has_value();
    }

    Project& Current() {
        if (!HasCurrent()) {
            throw std::runtime_error("no current project!");
        }
        return current.value();
    }

    void AddProjectAsCurrent(const Project& project) {
        current = AddProject(project);
    }

    Project& AddProject(const Project& project) {
        projects.push_back(project);
        return projects.at(projects.size() - 1);
    }

private:
    vector<Project> projects;
    optional_ref<Project> current;
};