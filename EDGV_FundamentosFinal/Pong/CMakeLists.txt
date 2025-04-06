cmake_minimum_required(VERSION 3.28)

add_executable(Pong
 src/main.cpp
 src/pongFramework.cpp
 src/scripts/gameManager.cpp
 src/scripts/gameMap.cpp
 src/scripts/mapBuilder.cpp
 src/scripts/mapTilesFactory.cpp
 src/scripts/statesPattern/stateMachine.cpp
 src/scripts/statesPattern/states/stateGameOver.cpp
 src/scripts/statesPattern/states/stateGamePaused.cpp
 src/scripts/statesPattern/states/stateGamePlaying.cpp
 src/scripts/statesPattern/states/stateGameStarting.cpp
 src/scripts/statesPattern/states/stateGameWon.cpp
 src/scripts/statesPattern/states/stateLoadGame.cpp
 src/scripts/statesPattern/states/stateMainMenu.cpp
 src/scripts/statesPattern/states/stateSaveGame.cpp
 src/scripts/mapEntities/collectable.cpp
 src/scripts/mapEntities/collectables/coin.cpp
 src/scripts/mapEntities/collectables/powerCoin.cpp
 src/scripts/mapEntities/moving/movingEntity.cpp
 src/scripts/mapEntities/moving/phantom.cpp
 src/scripts/mapEntities/moving/player.cpp
 src/scripts/mapEntities/moving/phantoms/blinky.cpp
 src/scripts/mapEntities/moving/phantoms/clyde.cpp
 src/scripts/mapEntities/moving/phantoms/inky.cpp
 src/scripts/mapEntities/moving/phantoms/pinky.cpp
)

set_target_properties(Pong PROPERTIES
 CXX_STANDARD 20
 CXX_STANDARD_REQUIRED YES
 CXX_EXTENSIONS NO
)

target_include_directories(Pong PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_dependencies(Pong Framework)
target_link_libraries(Pong PRIVATE Framework SFML::Graphics)