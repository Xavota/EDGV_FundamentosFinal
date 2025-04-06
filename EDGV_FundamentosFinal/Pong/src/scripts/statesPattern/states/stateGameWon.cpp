#include "scripts/statesPattern/states/stateGameWon.h"

#include <tools/time.h>
#include <tools/input.h>

#include <scene/actor.h>

#include "scripts/gameManager.h"
#include "scripts/gameMap.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGameWon::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  gameManager.lock()->pause();
  gameManager.lock()->m_pGameWonMenu->setActive(true);
}

U8 StateGameWon::update(WPtr<GameManager> gameManager)
{
  return 0;
}

U8 StateGameWon::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  if (input == eEXTERNAL_INPUT::kToMainMenu) {
    gameManager.lock()->m_pGameMap->getActor().lock()->setActive(false);
    return eSTATE_INDEX::kMainMenu;
  }
  else if (input == eEXTERNAL_INPUT::kRestartGame) {
    gameManager.lock()->newGame();
    return eSTATE_INDEX::kGameStarting;
  }
  return 0;
}

void StateGameWon::exit(WPtr<GameManager> gameManager)
{
  gameManager.lock()->m_pGameWonMenu->setActive(false);
}