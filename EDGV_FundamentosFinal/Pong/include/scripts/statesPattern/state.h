#pragma once
#include <platform/stdHeaders.h>

class StateMachine;
class GameManager;

class State
{
 public:
  /**
   * @brief Default constructor.
   */
  State() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~State() = default;

  virtual void init(SPtr<StateMachine> machine) { m_pStateMachine = machine; }

  virtual void enter(U8 lastState, WPtr<GameManager> gameManager) {}
  virtual U8 update(WPtr<GameManager> gameManager) { return 0; }
  virtual void exit(WPtr<GameManager> gameManager) {}
  virtual U8 externalInput(U8 input, WPtr<GameManager> gameManager) { return 0; }

 protected:
  SPtr<StateMachine> m_pStateMachine = nullptr;
};