//
// Created by Tobiathan on 1/3/23.
//

#include <fstream>
#include "Styles.h"
#include "Style.h"
#include "CssParser.h"

void Styles::Initialize() {
    instance = std::make_unique<Styles>();
}

Style &Styles::Get(const string& name) {
    if (!styleMap.contains(name)) {
        throw std::runtime_error("`" + name + "` not contained in styles!");
    }
    return styleMap[name];
}

void Styles::ResetCss() {
    Instance().styleMap = {};
}

void Styles::LoadCss(const string& cssFileContents) {
    for (const ClassStyle& classStyle : CssParser::CssFileContentsToStyles(cssFileContents)) {
        Instance().styleMap[classStyle.className].AddToTop(classStyle.style);
    }
}

void Styles::LoadCssFile(const path& filePath) {

    if (!exists(filePath)) {
        Log("[WARNING]: not found, failed to load CSS from: `{}`", filePath);
        return;
    }
    std::ifstream inputStream(filePath);
    std::stringstream inputBuffer;
    inputBuffer << inputStream.rdbuf();
    LoadCss(inputBuffer.str());
}

void Styles::LoadCssDirectory(const path& dirPath) {
    LoadCssFiles(
        directory_iterator{ dirPath }
        | transform([](auto& entry){ return entry.path(); })
        | to_vector()
    );
}

void Styles::LoadCssFiles(const vector<path>& filePaths) {
    for (const auto& filePath : filePaths) {
        LoadCssFile(filePath);
    }
}
