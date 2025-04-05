#pragma once

#include "scripts/statesPattern/state.h"

class StateSaveGame : public State
{
 public:
  /**
   * @brief Default constructor.
   */
  StateSaveGame() = default;
  /**
   * @brief Default destructor.
   */
  ~StateSaveGame() = default;

  void enter(U8 lastState, WPtr<GameManager> gameManager) override;
  U8 update(WPtr<GameManager> gameManager) override;
  U8 externalInput(U8 input, WPtr<GameManager> gameManager) override;
};