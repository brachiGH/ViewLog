cmake_minimum_required(VERSION 3.18)

project(ViewLogApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Gui Qml Quick)
qt_add_executable(ViewLogApp src/main.cpp
    src/filestreemodel.h src/filestreemodel.cpp)

qt_add_resources(ViewLogApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
        assets/images/bar_right_icon.svg
        assets/images/collapse-Tree.png
        assets/images/controlvideos.svg
        assets/images/down_icon.svg
        assets/images/exitfullscreen.svg
        assets/images/file.svg
        assets/images/folder.svg
        assets/images/fullscreen.svg
        assets/images/next-icon.svg
        assets/images/nextvideo.svg
        assets/images/pause.svg
        assets/images/picture_in_enter_icon.svg
        assets/images/picture_in_exit_icon.svg
        assets/images/play.svg
        assets/images/seekback.svg
        assets/images/seekforward.svg
        assets/images/show-Tree.png
        assets/images/speed.svg
        assets/images/subtitles.svg
        assets/images/volume-1.svg
        assets/images/volume-2.svg
        assets/images/volume-3.svg
        assets/images/volume-muted.svg
        assets/treeWidget/stylesheet-vline.png
        assets/treeWidget/stylesheet-branch-open.png
        assets/treeWidget/stylesheet-branch-more.png
        assets/treeWidget/stylesheet-branch-end.png
        assets/treeWidget/stylesheet-branch-closed.
        
)

target_link_libraries(ViewLogApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
)

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

