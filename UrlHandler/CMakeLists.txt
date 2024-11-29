add_library(UrlHandler STATIC)

target_sources(UrlHandler
        PRIVATE
        InputHelper.cpp
)

target_include_directories(UrlHandler
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UrlHandler
        PRIVATE
        Qt6::Core
)