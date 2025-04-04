#pragma once

#include <scene/components/script.h>

class GameMap;
class Render;

class MovingEntity : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  MovingEntity() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~MovingEntity() = default;

  virtual void setCanMove(bool can);

 protected:
  virtual U8 getMovementDecision(const sf::Vector2i& dir, U8 options) { return 0; };

  virtual void moveAnimation();
  virtual String getAnimationName() const { return ""; }
  virtual void setAnimationFrame(const sf::Vector2i& dir, U8 frame);

  virtual void move(U8 options);

  void update() override;


  WPtr<GameMap> m_pGameMap;
  float m_fOgSpeed;
  float m_fSpeed;

  sf::Vector2i m_movementDir;
  sf::Vector2u m_mapPos;
  sf::Vector2f m_posOffset;

  sf::Vector2i m_ogMovementDir;
  sf::Vector2u m_ogMapPos;
  sf::Vector2f m_ogPosOffset;

  bool m_bCanMove = false;

  SPtr<Render> m_pRenderComp;
  U32 m_iMaxAnimationFrames = 3;
  U32 m_iCurrentAnimationFrame = 0;
  float m_fMaxFrameTime = 0.08f;
  float m_iCurrentFrameTime = 0.0f;
};