include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY git@github.com:nlohmann/json.git
        GIT_TAG        9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03 # release-3.11.3
)
FetchContent_MakeAvailable(json)

find_package(CURL REQUIRED)

add_library(IpStackWrapper STATIC)

target_sources(IpStackWrapper
        PRIVATE
        IpStackWrapper.cpp
)

target_include_directories(IpStackWrapper
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(IpStackWrapper
        PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
        Qt6::Core
)