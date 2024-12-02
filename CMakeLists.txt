cmake_minimum_required(VERSION 3.22)
project(Sofomo LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

add_subdirectory(Src)

add_subdirectory(Tests)
