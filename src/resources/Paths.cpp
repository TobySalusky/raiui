//
// Created by Tobiathan on 10/20/22.
//

#include "Paths.h"

std::string Paths::Asset(const std::string& subPathFromAssetsDir) {
    return assetRoot + subPathFromAssetsDir;
}

std::string Paths::Image(const std::string& subPathFromImagesDir) {
    return assetRoot + "images/"s + subPathFromImagesDir;
}

std::string Paths::Font(const string& subPathFromImagesDir) {
    return assetRoot + "fonts/"s + subPathFromImagesDir;
}
