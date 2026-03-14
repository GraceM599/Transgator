# CMake from sfml recommended github and help from ChatGPT
cmake_minimum_required(VERSION 3.28)
project(TRANSGATOR LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)

set(SFML_STATIC_LIBRARIES OFF)  # we are using dynamic SFML
add_executable(TRANSGATOR ${PROJECT_SOURCES}
        src/sfmlhelpers.cpp)

target_compile_features(TRANSGATOR PRIVATE cxx_std_17)

target_link_libraries(TRANSGATOR PRIVATE SFML::Graphics SFML::Window SFML::System)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TRANSGATOR)