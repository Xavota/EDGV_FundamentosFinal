#include "scripts/statesPattern/stateMachine.h"

#include <platform/memoryManager.hpp>

#include "scripts/statesPattern/states/stateGameOver.h"
#include "scripts/statesPattern/states/stateGamePaused.h"
#include "scripts/statesPattern/states/stateGamePlaying.h"
#include "scripts/statesPattern/states/stateGameStarting.h"
#include "scripts/statesPattern/states/stateGameWon.h"
#include "scripts/statesPattern/states/stateMainMenu.h"

void StateMachine::init(WPtr<GameManager> manager)
{
  m_pGameManager = manager;

  m_mStates[eSTATE_INDEX::kMainMenu] = MemoryManager::createShared<StateMainMenu>();
  m_mStates[eSTATE_INDEX::kGameStarting] = MemoryManager::createShared<StateGameStarting>();
  m_mStates[eSTATE_INDEX::kGamePlaying] = MemoryManager::createShared<StateGamePlaying>();
  m_mStates[eSTATE_INDEX::kGamePaused] = MemoryManager::createShared<StateGamePaused>();
  m_mStates[eSTATE_INDEX::kGameOver] = MemoryManager::createShared<StateGameOver>();
  m_mStates[eSTATE_INDEX::kGameWon] = MemoryManager::createShared<StateGameWon>();

  m_mStates[m_iCurrentState]->enter(0, m_pGameManager);
}

void StateMachine::update()
{
  U8 newState = m_mStates[m_iCurrentState]->update(m_pGameManager);
  changeState(newState);
}

void StateMachine::externalInput(U8 input)
{
  U8 newState = m_mStates[m_iCurrentState]->externalInput(input, m_pGameManager);
  changeState(newState);
}

void StateMachine::changeState(U8 newState)
{
  if (newState != 0 && newState != m_iCurrentState) {
    m_mStates[m_iCurrentState]->exit(m_pGameManager);
    m_mStates[newState]->enter(m_iCurrentState, m_pGameManager);
    m_iCurrentState = newState;
  }
}