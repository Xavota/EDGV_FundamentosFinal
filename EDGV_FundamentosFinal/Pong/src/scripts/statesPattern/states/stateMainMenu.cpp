#include "scripts/statesPattern/states/stateMainMenu.h"

#include <tools/time.h>
#include <tools/input.h>

#include <scene/actor.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateMainMenu::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  gameManager.lock()->m_pMainMenu->setActive(true);
}

U8 StateMainMenu::update(WPtr<GameManager> gameManager)
{
  return 0;
}

U8 StateMainMenu::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  if (input == eEXTERNAL_INPUT::kStartGame) {
    return eSTATE_INDEX::kGameStarting;
  }
  return 0;
}

void StateMainMenu::exit(WPtr<GameManager> gameManager)
{
  gameManager.lock()->m_pMainMenu->setActive(false);
}