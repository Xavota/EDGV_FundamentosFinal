cmake_minimum_required(VERSION 3.28)

project(FinalProject LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /DDEBUG")
    message("Using MSVC")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# target_compile_features(FinalProject PRIVATE cxx_std_17)
# target_link_libraries(FinalProject PRIVATE SFML::Graphics)

# target_include_directories(FinalProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(Framework)
add_subdirectory(Pong)