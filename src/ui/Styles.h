//
// Created by Tobiathan on 1/3/23.
//

#ifndef ARTNET_STYLES_H
#define ARTNET_STYLES_H


#include "Includes.h"
#include <filesystem>

using namespace std::filesystem;
using namespace tutil;

class Style;

class Styles {
public:
    static void Initialize();
    static Styles& Instance() { return *instance; }
    Style& Get(const string& name);

    static void LoadCssDirectory(const path& dirPath);
    static void LoadCssFiles(const vector<path>& filePaths);
    static void LoadCssFile(const path& filePath);
    static void LoadCss(const string& cssFileContents);
    static void ResetCss();

private:
    unordered_map<string, Style> styleMap = {};
    static inline unique_ptr<Styles> instance = {};
};


#endif //ARTNET_STYLES_H
