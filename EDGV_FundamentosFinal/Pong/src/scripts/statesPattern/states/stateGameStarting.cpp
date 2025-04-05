#include "scripts/statesPattern/states/stateGameStarting.h"

#include <tools/time.h>
#include <tools/input.h>

#include "scripts/gameManager.h"
#include "scripts/statesPattern/stateMachine.h"

void StateGameStarting::enter(U8 lastState, WPtr<GameManager> gameManager)
{
  if (lastState == eSTATE_INDEX::kGamePaused) return;
  
  m_fTimeToStart = 0.0f;
  gameManager.lock()->prepareGame();
}

U8 StateGameStarting::update(WPtr<GameManager> gameManager)
{
  m_fTimeToStart += gl::Time::instance().deltaTime();
  if (m_fTimeToStart >= m_fMaxTimeToStart) {
    return eSTATE_INDEX::kGamePlaying;
  }
  if (gl::Input::getKeyPressed(sf::Keyboard::Key::Escape)) {
    return eSTATE_INDEX::kGamePaused;
  }

  return 0;
}