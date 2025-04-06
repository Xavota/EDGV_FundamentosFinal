#include "scripts/statesPattern/states/stateGamePaused.h"

#include <tools/time.h>
#include <tools/input.h>

#include <scene/actor.h>

#include "scripts/gameManager.h"
#include "scripts/gameMap.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGamePaused::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  m_iLastState = lastState;
  gameManager.lock()->pause();
  gameManager.lock()->m_pPauseMenu->setActive(true);
}

U8 StateGamePaused::update(WPtr<GameManager> gameManager)
{
  if (gl::Input::getKeyPressed(sf::Keyboard::Key::Escape)) {
    return m_iLastState == 0 ? eSTATE_INDEX::kGamePlaying : m_iLastState;
  }
  return 0;
}

U8 StateGamePaused::externalInput(U8 input, WPtr<GameManager> gameManager)
{
  if (input == eEXTERNAL_INPUT::kResumeGame) {
    return m_iLastState == 0 ? eSTATE_INDEX::kGamePlaying : m_iLastState;
  }
  else if (input == eEXTERNAL_INPUT::kToMainMenu) {
    gameManager.lock()->m_pGameMap->getActor().lock()->setActive(false);
    return eSTATE_INDEX::kMainMenu;
  }
  return 0;
}

void StateGamePaused::exit(WPtr<GameManager> gameManager)
{
  gameManager.lock()->m_pPauseMenu->setActive(false);
}