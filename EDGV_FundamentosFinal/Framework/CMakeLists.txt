cmake_minimum_required(VERSION 3.28)
project(Framework LANGUAGES CXX)

add_library(Framework STATIC
  src/frameworkManager.cpp
  src/platform/math/matrix3.cpp
  src/scene/actor.cpp
  src/scene/scene.cpp
  src/scene/sceneManager.cpp
  src/scene/components/transform.cpp
  src/tools/font.cpp
  src/tools/input.cpp
  src/tools/math.cpp
  src/tools/texture.cpp
  src/tools/time.cpp
)

message("Framework cmake source dir: " ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Framework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(Framework PRIVATE SFML::Graphics)

set_target_properties(Framework PROPERTIES
 CXX_STANDARD 20
 CXX_STANDARD_REQUIRED YES
 CXX_EXTENSIONS NO
)

# target_compile_features(Framework PRIVATE cxx_std_20)