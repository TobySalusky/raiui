cmake_minimum_required(VERSION 3.24)
project(ArtNet)

set(CMAKE_CXX_STANDARD 23)
string(APPEND CMAKE_EXE_LINKER_FLAGS " -fmodules-ts")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-psabi) # ignore warnings about unimportant gcc abi changes (raylib src)
endif()

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 4.2.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib_cpp
            GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
            GIT_TAG v4.2.6
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

# This is the main part:
set(CUSTOM_INCLUDES ./includes)
include_directories(${CUSTOM_INCLUDES})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS system filesystem)

set(SOURCES src/main.cpp includes/Includes.h src/resources/Paths.h src/resources/Paths.cpp src/resources/Textures.h src/resources/Textures.cpp src/Program.cpp src/Program.h src/resources/Fonts.cpp src/resources/Fonts.h src/util/Logger.cpp src/util/Logger.h src/ui/UIElement.h src/ui/UIElement.cpp src/ui/Button.h src/ui/DOM.cpp src/ui/DOM.h src/ui/Main.h src/ui/Main.cpp src/ui/Text.h src/ui/Style.cpp src/ui/Style.h src/ui/Styles.cpp src/ui/Styles.h src/ui/Identification.cpp src/ui/Identification.h src/ui/ScopeId.cpp src/ui/ScopeId.h src/ui/Flex.h src/ui/Div.h src/ui/Span.h src/ui/Panel.h src/ui/State.cpp src/ui/State.h src/ui/Units.h src/ui/Slot.cpp src/ui/Slot.h src/ui/CenterPoint.h src/util/Util.cpp src/util/Util.h src/ui/CssParser.cpp src/ui/CssParser.h includes/magic_enum.h src/ui/ColorConverter.cpp src/ui/ColorConverter.h src/ui/Tooltip.cpp src/ui/Tooltip.h src/ui/UIComponent.h src/ui/ScrollViewY.cpp src/ui/ScrollViewY.h src/ui/FilePicker.cpp src/ui/FilePicker.h src/ui/Interactive.h src/ui/Text.cpp src/ui/FilePickerDirectory.cpp src/ui/FilePickerDirectory.h src/ui/AppState.cpp src/ui/AppState.h src/ui/FilePickerDirectoryNode.cpp src/ui/FilePickerDirectoryNode.h src/ui/OptStyles.cpp src/ui/OptStyles.h src/ui/tloc.cpp src/ui/tloc.h src/ui/ImageLeaf.h src/ui/ImageLeaf.cpp src/ui/Leaf.cpp src/ui/Leaf.h src/artnet/Canvas.cpp src/artnet/Canvas.h src/artnet/User.cpp src/artnet/User.h src/artnet/Brush.cpp src/artnet/Brush.h src/artnet/CanvasView.cpp src/artnet/CanvasView.h src/ui/TextInput.cpp src/ui/TextInput.h includes/UIEssentials.h includes/UICommon.h src/ui/KeyInput.cpp src/ui/KeyInput.h src/ui/KeyBinds.cpp src/ui/KeyBinds.h src/artnet/pages/SaveModal.h src/artnet/pages/SaveModal.cpp src/ui/FilePathInput.h src/ui/FilePathInput.cpp src/artnet/ColorPicker.h src/artnet/ColorPicker.cpp src/ui/SizeRoutine.h src/ui/SizeRoutine.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
# TODO: --> 23??
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raylib_cpp Boost::filesystem)

set(CMAKE_C_FLAGS_ASAN "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1")
set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1")

set(CMAKE_C_FLAGS_UBSAN "-fsanitize=undefined")
set(CMAKE_CXX_FLAGS_UBSAN "-fsanitize=undefined")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# That's it! You should have an example executable that you can run. Have fun!
