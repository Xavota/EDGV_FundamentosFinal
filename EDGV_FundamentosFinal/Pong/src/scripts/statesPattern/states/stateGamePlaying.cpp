#include "scripts/statesPattern/states/stateGamePlaying.h"

#include <tools/time.h>
#include <tools/input.h>

#include <scene/actor.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGamePlaying::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  gameManager.lock()->m_pGameMenu->setActive(true);
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
    if (gameManager.lock()->isGameOver()) {
      return eSTATE_INDEX::kGameOver;
    }
    else {
      gameManager.lock()->resetMovingObj();
      return eSTATE_INDEX::kGameStarting;
    }
  }
  else if (input == eEXTERNAL_INPUT::kGotLastCoin) {
    return eSTATE_INDEX::kGameWon;
  }
  return 0;
}

void StateGamePlaying::exit(WPtr<GameManager> gameManager)
{
  gameManager.lock()->m_pGameMenu->setActive(false);
}