#include "scripts/statesPattern/states/stateGameOver.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGameOver::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  // Open lose game menu
}

U8 StateGameOver::update(WPtr<GameManager> gameManager)
{
  return 0;
}

U8 StateGameOver::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  if (input == eEXTERNAL_INPUT::kToMainMenu) {
    return eSTATE_INDEX::kMainMenu;
  }
  else if (input == eEXTERNAL_INPUT::kRestartGame) {
    return eSTATE_INDEX::kGameStarting;
  }
  return 0;
}

void StateGameOver::exit(WPtr<GameManager> gameManager)
{
  // Close lose game menu
}