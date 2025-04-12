cmake_minimum_required(VERSION 3.28)

add_library(SpeedMod SHARED
 src/main.cpp
)

set_target_properties(SpeedMod PROPERTIES
 CXX_STANDARD 20
 CXX_STANDARD_REQUIRED YES
 CXX_EXTENSIONS NO
)

target_include_directories(SpeedMod PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(SpeedMod PRIVATE Framework SFML::Graphics)

add_dependencies(SpeedMod Framework Pong)

target_compile_definitions(SpeedMod PRIVATE SPEED_MOD_DLL_EXPORTS)