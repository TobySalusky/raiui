//
// Created by Tobiathan on 10/20/22.
//

#ifndef MARIOGAME_PATHS_H
#define MARIOGAME_PATHS_H

#include "Includes.h"

class Paths {
public:
    static std::string Asset(const std::string& subPathFromAssetsDir);
    static std::string Image(const std::string& subPathFromImagesDir);
    static std::string Font(const std::string& subPathFromImagesDir);

private:
    static constexpr const char* assetRoot = "../assets/";
};


#endif //MARIOGAME_PATHS_H
