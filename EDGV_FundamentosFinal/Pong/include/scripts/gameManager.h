#pragma once

#include <scene/components/script.h>

class GameMap;
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

  void prepareGame();
  void startPlaying();
  void pause();
  void resume();

 private:
  void start() override;

  void eatablePhantoms();
  void playerDied();
  void resetGame();

  SPtr<GameMap> m_pGameMap;

  SPtr<Player> m_pPlayer;
  SPtr<Blinky> m_pBlinky;
  SPtr<Pinky> m_pPinky;
  SPtr<Inky> m_pInky;
  SPtr<Clyde> m_pClyde;
};