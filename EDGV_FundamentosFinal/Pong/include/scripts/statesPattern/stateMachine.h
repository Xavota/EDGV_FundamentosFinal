#pragma once

#include <scene/components/script.h>

class State;
class GameManager;

namespace eSTATE_INDEX
{
  enum E : U8
  {
    kMainMenu = 1,
    kGameStarting,
    kGamePlaying,
    kGamePaused,
    kGameOver,
    kGameWon,
  };
}

namespace eEXTERNAL_INPUT
{
  enum E : U8
  {
    kStartGame,
    kGotLastCoin,
    kPlayerDied,
    kToMainMenu,
    kRestartGame,
    kResumeGame,
  };
}

class StateMachine : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  StateMachine() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~StateMachine() = default;

  virtual void init(WPtr<GameManager> manager);
  virtual void update();
  virtual void externalInput(U8 input);

  virtual void changeState(U8 newState);

 private:
  WPtr<GameManager> m_pGameManager;
  Map<U8, SPtr<State>> m_mStates = {};
  U8 m_iCurrentState = 1;
};