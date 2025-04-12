cmake_minimum_required(VERSION 3.28)

add_library(NewMapMod SHARED
 src/main.cpp
)

set_target_properties(NewMapMod PROPERTIES
 CXX_STANDARD 20
 CXX_STANDARD_REQUIRED YES
 CXX_EXTENSIONS NO
)

message("NewMapMod cmake binary dir: " ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(NewMapMod PRIVATE
 ${CMAKE_CURRENT_SOURCE_DIR}/include/
 ${CMAKE_CURRENT_BINARY_DIR}/../../Pong/include/
)
target_link_libraries(NewMapMod PRIVATE Framework SFML::Graphics)

add_dependencies(NewMapMod Framework Pong)

target_compile_definitions(NewMapMod PRIVATE NEW_MAP_MOD_DLL_EXPORTS)