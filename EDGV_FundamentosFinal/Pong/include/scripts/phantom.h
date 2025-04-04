#pragma once

#include "scripts/movingEntity.h"

class Phantom : public MovingEntity
{
 public:
  /**
   * @brief Default constructor.
   */
  Phantom() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~Phantom() = default;

  void init(WPtr<GameMap> map, float speed, U8 phantomIndex);

 protected:
  U8 getMovementDecision(const sf::Vector2i& dir, U8 options) override;

  sf::Vector2u m_lastPos;

  //void update() override;
};