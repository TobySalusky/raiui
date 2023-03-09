////
//// Created by Tobiathan on 2/24/23.
////
//
#include "FilePicker.h"
#include "ScrollViewY.h"
#include "FilePickerDirectory.h"
#include "FilePickerDirectoryNode.h"
#include "Text.h"

tui::FilePicker::FilePicker(const string& topLevelDirPath, const tui::id_t& idLike,
                            std::source_location location) : UIComponent(idLike, location) {
    FilePickerDirectoryNode& filePickerRoot = UseInitRef([&](){
        return FilePickerDirectoryNode::Create(topLevelDirPath);
    });

    Div scrollView("file-picker-container");
//    ScrollViewY scrollView("file-picker-container"); // TODO: crashes rn :'(
    Text(strong{ "Project" }, "file-picker-header");
    FilePickerDirectory topDir(filePickerRoot);
}