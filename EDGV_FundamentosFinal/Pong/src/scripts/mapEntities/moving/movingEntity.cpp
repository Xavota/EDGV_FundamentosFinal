#include "scripts/mapEntities/moving/movingEntity.h"

#include <platform/math.h>

#include <tools/time.h>
#include <tools/textureManager.h>

#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/render.h>

#include "scripts/gameMap.h"

void MovingEntity::setCanMove(bool can)
{
  m_bCanMove = can;
}

void MovingEntity::move(U8 options)
{
  m_posOffset += sf::Vector2f(m_movementDir) * m_fSpeed *
                 gl::Time::instance().deltaTime();

  if (std::abs(m_posOffset.x) > 1.0f) {
    float sign = (m_posOffset.x / std::abs(m_posOffset.x));
    m_posOffset.x -= 1.0f * sign;
    m_mapPos.x += static_cast<U32>(sign);
  }
  if (std::abs(m_posOffset.y) > 1.0f) {
    float sign = (m_posOffset.y / std::abs(m_posOffset.y));
    m_posOffset.y -= 1.0f * sign;
    m_mapPos.y += static_cast<U32>(sign);
  }
  //std::cout << "m_posOffset(" << m_posOffset.x << ", " << m_posOffset.y << ")" << std::endl;
  //std::cout << "m_mapPos(" << m_mapPos.x << ", " << m_mapPos.y << ")" << std::endl;

  bool canMoveUp = Math::hasFlag(options, eMOVEMENT_DIR::kUp);
  bool canMoveRight = Math::hasFlag(options, eMOVEMENT_DIR::kRight);
  bool canMoveDown = Math::hasFlag(options, eMOVEMENT_DIR::kDown);
  bool canMoveLeft = Math::hasFlag(options, eMOVEMENT_DIR::kLeft);

  if (m_movementDir.y == -1 && !canMoveUp) {
    m_posOffset = {};
  }
  else if (m_movementDir.x == 1 && !canMoveRight) {
    m_posOffset = {};
  }
  else if (m_movementDir.y == 1 && !canMoveDown) {
    m_posOffset = {};
  }
  else if (m_movementDir.x == -1 && !canMoveLeft) {
    m_posOffset = {};
  }
}

void MovingEntity::moveAnimation()
{
  m_iCurrentFrameTime += gl::Time::instance().deltaTime();
  if (m_iCurrentFrameTime >= m_fMaxFrameTime) {
    m_iCurrentFrameTime -= m_fMaxFrameTime;
    ++m_iCurrentAnimationFrame;
    m_iCurrentAnimationFrame %= m_iMaxAnimationFrames;

    setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);
  }
}

void MovingEntity::setAnimationFrame(const sf::Vector2i& dir, U8 frame)
{
  String animationName = getAnimationName();

  if (animationName.empty()) {
    m_pRenderComp->m_material.m_pTexture =
    gl::TextureManager::instance().getTexture("error");
  }
  else {
    String renderFrameLetter = "";
    if (dir.y == -1) {
      renderFrameLetter = "_u";
    }
    else if (dir.x == 1) {
      renderFrameLetter = "_r";
    }
    else if (dir.y == 1) {
      renderFrameLetter = "_d";
    }
    else if (dir.x == -1) {
      renderFrameLetter = "_l";
    }

    m_pRenderComp->m_material.m_pTexture =
     gl::TextureManager::instance().getTexture(
      animationName + renderFrameLetter + std::to_string(frame)
    );
  }
}

void MovingEntity::update()
{
  if (m_pGameMap.expired()) return;

  SPtr<GameMap> map = m_pGameMap.lock();
  if (m_bCanMove) {
    //std::cout << "GettingMovementOption " << getActor().lock()->getName() << std::endl;
    U8 options = map->getMovementOptions(m_mapPos);
    //std::cout << "GotMovementOption " << getActor().lock()->getName() << std::endl;

    move(options);

    options = map->getMovementOptions(m_mapPos);
    U8 decision = getMovementDecision(m_movementDir, options);

    bool moveUp = Math::hasFlag(decision, eMOVEMENT_DIR::kUp);
    bool moveRight = Math::hasFlag(decision, eMOVEMENT_DIR::kRight);
    bool moveDown = Math::hasFlag(decision, eMOVEMENT_DIR::kDown);
    bool moveLeft = Math::hasFlag(decision, eMOVEMENT_DIR::kLeft);

    if (moveUp) {
      m_movementDir = {0, -1};
      m_posOffset.x = 0.0f;
      setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);
    }
    if (moveRight) {
      m_movementDir = {1, 0};
      m_posOffset.y = 0.0f;
      setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);
    }
    if (moveDown) {
      m_movementDir = {0, 1};
      m_posOffset.x = 0.0f;
      setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);
    }
    if (moveLeft) {
      m_movementDir = {-1, 0};
      m_posOffset.y = 0.0f;
      setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);
    }

    SPtr<Transform> transform = getTransform().lock();
    transform->setLocalPosition(
      static_cast<sf::Vector2f>(m_mapPos) + m_posOffset);

    moveAnimation();
  }
}
