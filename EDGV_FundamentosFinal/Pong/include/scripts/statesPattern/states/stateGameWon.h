#pragma once

#include "scripts/statesPattern/state.h"

class StateGameWon : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateGameWon() = default;
  /**
   * @brief Default destructor.
   */
  ~StateGameWon() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;
  U8 externalInput(U8 input, WPtr<GameManager> gameManager) override;
  void exit(WPtr<GameManager> gameManager) override;
};