#pragma once

#include "scripts/mapEntities/moving/movingEntity.h"

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

  virtual void init(WPtr<GameMap> map, float speed, U8 phantomIndex);

  virtual void makeEatable(float time);
  virtual void makeNotEatable();
  virtual void eaten();
  virtual void reset();

 protected:
  U8 getMovementDecision(const sf::Vector2i& dir, U8 options) override;

  void setAnimationFrame(const sf::Vector2i& dir, U8 frame) override;

  void move(U8 options) override;

  void onCollisionEnter(CollisionInfo info) override;


  void update() override;

  
  sf::Vector2u m_lastPos;

  bool m_bIsEatable = false;
  bool m_bIsBeenEaten = false;

  float m_fEatableMaxTime;
  float m_fEatableTime;
};