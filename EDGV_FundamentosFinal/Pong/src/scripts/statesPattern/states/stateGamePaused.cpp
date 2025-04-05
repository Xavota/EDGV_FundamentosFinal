#include "scripts/statesPattern/states/stateGamePaused.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGamePaused::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  m_iLastState = lastState;
  gameManager.lock()->pause();
  //Open pause menu
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
  return 0;
}

void StateGamePaused::exit(WPtr<GameManager> gameManager)
{
  // Close pause menu
}