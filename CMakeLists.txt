cmake_minimum_required(VERSION 3.1.13)

project(sighan
    VERSION 0.1.0
    LANGUAGES CXX
)

option(BUILD_SIGHAN_TESTS "Build and perform sighan tests")
option(BUILD_SIGHAN_EXAMPLES "Build sighan examples")

add_library(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME}
    PUBLIC
        cxx_std_11
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        COMPILE_FLAGS "-Werror -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/sighan.cpp
)

include(FetchContent)

FetchContent_Declare(optional-lite
    GIT_REPOSITORY https://github.com/martinmoene/optional-lite
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(optional-lite)

target_link_libraries(${PROJECT_NAME} optional-lite)

if (BUILD_SIGHAN_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (BUILD_SIGHAN_EXAMPLES)
    add_subdirectory(examples)
endif()
