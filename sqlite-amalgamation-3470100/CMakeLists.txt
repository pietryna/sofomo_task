add_library(sqlite STATIC)

target_sources(sqlite
        PRIVATE
        sqlite3.c
)

target_include_directories(sqlite
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)