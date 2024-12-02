cmake_minimum_required(VERSION 3.22)
project(Sofomo LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

add_subdirectory(sqlite-amalgamation-3470100)
add_subdirectory(DbHandler)
add_subdirectory(UrlHandler)
add_subdirectory(IpStackWrapper)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        main.cpp
        mainwindow.cpp
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        DbHandler
        UrlHandler
        IpStackWrapper
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        -DMAIN_WINDOW="${PROJECT_NAME}"
)

add_subdirectory(Tests)
