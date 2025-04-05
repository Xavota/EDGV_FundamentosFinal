#include "scripts/statesPattern/states/stateGamePlaying.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGamePlaying::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  if (lastState == eSTATE_INDEX::kGamePaused) {
    gameManager.lock()->resume();
  }
  else {
    gameManager.lock()->startPlaying();
  }
}

U8 StateGamePlaying::update(WPtr<GameManager> gameManager)
{
  if (gl::Input::getKeyPressed(sf::Keyboard::Key::Escape)) {
    return eSTATE_INDEX::kGamePaused;
  }
  return 0;
}

U8 StateGamePlaying::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  if (input == eEXTERNAL_INPUT::kPlayerDied) {
    return eSTATE_INDEX::kGameOver;
  }
  else if (input == eEXTERNAL_INPUT::kGotLastCoin) {
    return eSTATE_INDEX::kGameWon;
  }
  return 0;
}