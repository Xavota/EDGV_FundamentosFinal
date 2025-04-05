#pragma once

#include "scripts/statesPattern/state.h"

class StateGameStarting : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateGameStarting() = default;
  /**
   * @brief Default destructor.
   */
  ~StateGameStarting() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;

  

 private:
  float m_fMaxTimeToStart = 5.0f;
  float m_fTimeToStart = 0.0f;
};