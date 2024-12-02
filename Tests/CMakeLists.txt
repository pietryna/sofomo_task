include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(mocks)

add_library(DbHandlerTest
        STATIC
        ../DbHandler/DbHandler.cpp
        ../DbHandler/DatabaseItem.cpp
        DbHandlerTest.cpp
)
target_link_libraries(DbHandlerTest
        PRIVATE
        gtest
        mocks
        Qt6::Core # Added to enable QDebug - no point in mocking console
)

add_library(IpStackWrapperTest
        STATIC
        ../IpStackWrapper/IpStackWrapper.cpp
        IpStackWrapperTest.cpp
)

target_link_libraries(IpStackWrapperTest
        PRIVATE
        gtest
        mocks
        nlohmann_json::nlohmann_json
        Qt6::Core # Added to enable QDebug - no point in mocking console
)

set(TEST_TARGET "${PROJECT_NAME}_test")
add_executable(${PROJECT_NAME}_test)
target_link_libraries(${TEST_TARGET}
        PRIVATE
        gtest_main
        -Wl,--whole-archive
        DbHandlerTest
        IpStackWrapperTest
        -Wl,--no-whole-archive
)