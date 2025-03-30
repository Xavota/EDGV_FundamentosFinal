cmake_minimum_required(VERSION 3.28)

add_executable(Pong
 src/main.cpp
 src/pongFramework.cpp
 src/scripts/movementScript.cpp
)

set_target_properties(Pong PROPERTIES
 CXX_STANDARD 20
 CXX_STANDARD_REQUIRED YES
 CXX_EXTENSIONS NO
)

target_include_directories(Pong PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_dependencies(Pong Framework)
target_link_libraries(Pong PRIVATE Framework SFML::Graphics)