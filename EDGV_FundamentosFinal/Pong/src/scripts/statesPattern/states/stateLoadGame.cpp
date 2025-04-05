#include "scripts/statesPattern/states/stateLoadGame.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateLoadGame::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  // Open main menu
}

U8 StateLoadGame::update(WPtr<GameManager> gameManager)
{
  return 0;
}

U8 StateLoadGame::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  return 0;
}