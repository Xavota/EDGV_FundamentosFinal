#pragma once

#include "scripts/statesPattern/state.h"

class StateGamePaused : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateGamePaused() = default;
  /**
   * @brief Default destructor.
   */
  ~StateGamePaused() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;
  U8 externalInput(U8 input, WPtr<GameManager> gameManager) override;
  void exit(WPtr<GameManager> gameManager) override;

 private:
  U8 m_iLastState = 0;
};