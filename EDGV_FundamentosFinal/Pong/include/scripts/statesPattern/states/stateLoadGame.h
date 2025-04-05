#pragma once

#include "scripts/statesPattern/state.h"

class StateLoadGame : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateLoadGame() = default;
  /**
   * @brief Default destructor.
   */
  ~StateLoadGame() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;
  U8 externalInput(U8 input, WPtr<GameManager> gameManager) override;
};