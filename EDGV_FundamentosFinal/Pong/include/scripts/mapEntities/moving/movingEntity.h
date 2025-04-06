#pragma once

#include <scene/components/script.h>

class GameMap;
class Render;
class File;

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
  virtual void setPaused(bool paused);

  virtual void saveToFile(File& saveFile);
  virtual void loadFromFile(File& loadFile);

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
  bool m_bPaused = false;

  SPtr<Render> m_pRenderComp;
  U32 m_iMaxAnimationFrames = 3;
  U32 m_iCurrentAnimationFrame = 0;
  float m_fMaxFrameTime = 0.08f;
  float m_fCurrentFrameTime = 0.0f;

  bool m_bWrapping = false;
  bool m_bEnteringWrapping = false;
  sf::Vector2u m_wrappingEntrancePos;
  sf::Vector2u m_wrappingExitPos;
  sf::Vector2i m_wrappingEntranceDir;
  sf::Vector2i m_wrappingExitDir;
};