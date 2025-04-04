#pragma once

#include "scripts/movingEntity.h"

class GameMap;
class Render;
class Coin;

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

  void init(WPtr<GameMap> map, float speed);

  void collectedCoin();
  void collectedPowerCoin();

 private:
  String getAnimationName() const override { return "player"; }

  U8 getMovementDecision(const sf::Vector2i& dir, U8 options) override;
};