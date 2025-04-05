#include "scripts/statesPattern/states/stateSaveGame.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateSaveGame::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  // Open main menu
}

U8 StateSaveGame::update(WPtr<GameManager> gameManager)
{
  return 0;
}

U8 StateSaveGame::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  return 0;
}