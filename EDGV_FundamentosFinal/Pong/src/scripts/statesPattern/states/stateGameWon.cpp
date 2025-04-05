#include "scripts/statesPattern/states/stateGameWon.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGameWon::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  // Open win game menu
}

U8 StateGameWon::update(WPtr<GameManager> gameManager)
{
  return 0;
}

U8 StateGameWon::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  if (input == eEXTERNAL_INPUT::kToMainMenu) {
    return eSTATE_INDEX::kMainMenu;
  }
  else if (input == eEXTERNAL_INPUT::kRestartGame) {
    return eSTATE_INDEX::kGameStarting;
  }
  return 0;
}

void StateGameWon::exit(WPtr<GameManager> gameManager)
{
  // Close win game menu
}