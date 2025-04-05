#pragma once

#include "scripts/statesPattern/state.h"

class StateMainMenu : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateMainMenu() = default;
  /**
   * @brief Default destructor.
   */
  ~StateMainMenu() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;
  U8 externalInput(U8 input, WPtr<GameManager> gameManager) override;
  void exit(WPtr<GameManager> gameManager) override;
};