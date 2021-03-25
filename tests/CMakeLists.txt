cmake_minimum_required(VERSION 3.13)

project(sighan_tests CXX)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 11
        COMPILE_FLAGS "-Werror -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        main.cpp
        test_simple.cpp
)

include(FetchContent)
FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/onqtam/doctest
    GIT_TAG 2.4.5
)
FetchContent_MakeAvailable(doctest)

target_link_libraries(${PROJECT_NAME} doctest sighan)
