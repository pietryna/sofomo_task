add_library(DbHandler STATIC)

target_sources(DbHandler
        PRIVATE
        DbHandler.cpp
        DatabaseItem.cpp
)

target_include_directories(DbHandler
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DbHandler
        PUBLIC
        Qt6::Core
        sqlite
)