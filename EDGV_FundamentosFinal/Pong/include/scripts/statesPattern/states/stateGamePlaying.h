#pragma once

#include "scripts/statesPattern/state.h"

class StateGamePlaying : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateGamePlaying() = default;
  /**
   * @brief Default destructor.
   */
  ~StateGamePlaying() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;
  U8 externalInput(U8 input, WPtr<GameManager> gameManager) override;
  void exit(WPtr<GameManager> gameManager) override;
};