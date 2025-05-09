#pragma once

#include "scripts/mapEntities/moving/movingEntity.h"

class GameMap;
class GameManager;
class Render;
class Coin;
class File;

class Player : public MovingEntity
{
 public:
  /**
   * @brief Default constructor.
   */
  Player() = default;
  /**
   * @brief Default destructor.
   */
  ~Player() = default;

  void saveToFile(File& saveFile) override;
  void loadFromFile(File& loadFile) override;

  void init(WPtr<GameMap> map, float speed,
            FunctionPtr<void> eatablePhantoms, FunctionPtr<void> died,
            FunctionPtr<void> finishedDying);

  void collectedCoin();
  void collectedPowerCoin();

  void dead();
  void eatPhantom();

  void reset();

 private:
  friend class GameManager;

  U8 getMovementDecision(const sf::Vector2i& dir, U8 options) override;

  String getAnimationName() const override { return "player"; }
  void setAnimationFrame(const sf::Vector2i& dir, U8 frame) override;

  void move(U8 options) override;


  void update() override;


  FunctionPtr<void> m_fpEatablePhantoms;
  FunctionPtr<void> m_fpDied;
  FunctionPtr<void> m_fpFinishedDying;

  bool m_bCollectedCoin = false;
  bool m_bCollectedCoinStep = false;

  bool m_bDead = false;

  float m_fTimeToStartDeadAnimation = 0.0f;
  float m_fMaxTimeToStartDeadAnimation = 1.0f;

  float m_fTimeToFinishDeadAnimation = 0.0f;
  float m_fMaxTimeToFinishDeadAnimation = 1.0f;

  sf::Vector2u m_lastPos;

  U32 m_iOgLives = 5;
  U32 m_iLives;
};