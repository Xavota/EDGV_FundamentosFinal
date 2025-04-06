#pragma once

#include <scene/components/script.h>

class GameMap;
class StateMachine;
class Player;
class Blinky;
class Pinky;
class Inky;
class Clyde;

class GameManager : public Script, public EnableSPtrFromThis<GameManager> 
{
 public:
  /**
   * @brief Default constructor.
   */
  GameManager() = default;
  /**
   * @brief Default destructor.
   */
  ~GameManager() = default;

  void newGame();
  void prepareGame();
  void startPlaying();
  void pause();
  void resume();
  void resetMovingObj();

  bool isGameOver();

  SPtr<GameMap> m_pGameMap;
  SPtr<StateMachine> m_pStateMachine;

  SPtr<Actor> m_pMainMenu;
  SPtr<Actor> m_pPauseMenu;
  SPtr<Actor> m_pGameMenu;
  SPtr<Actor> m_pGameOverMenu;
  SPtr<Actor> m_pGameWonMenu;

 private:
  void start() override;

  void eatablePhantoms();
  void playerDied();


  void makeMainMenu(WPtr<Transform> canvasT);
  void makePauseMenu(WPtr<Transform> canvasT);
  void makeGameMenu(WPtr<Transform> canvasT);
  void makeGameOverMenu(WPtr<Transform> canvasT);
  void makeGameWonMenu(WPtr<Transform> canvasT);


  SPtr<Player> m_pPlayer;
  SPtr<Blinky> m_pBlinky;
  SPtr<Pinky> m_pPinky;
  SPtr<Inky> m_pInky;
  SPtr<Clyde> m_pClyde;
};