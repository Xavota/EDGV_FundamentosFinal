#include "scripts/mapEntities/moving/movingEntity.h"

#include <platform/math.h>
#include <platform/iofile.h>

#include <tools/time.h>
#include <tools/textureManager.h>

#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/render.h>

#include "scripts/gameMap.h"

void MovingEntity::saveToFile(File& saveFile)
{
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_fOgSpeed), sizeof(float));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_fSpeed), sizeof(float));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_movementDir), sizeof(sf::Vector2i));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_mapPos), sizeof(sf::Vector2u));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_posOffset), sizeof(sf::Vector2f));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_ogMovementDir), sizeof(sf::Vector2i));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_ogMapPos), sizeof(sf::Vector2u));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_ogPosOffset), sizeof(sf::Vector2f));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_bCanMove), sizeof(bool));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_bPaused), sizeof(bool));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_iMaxAnimationFrames), sizeof(U32));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_iMaxAnimationFrames), sizeof(U32));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_fMaxFrameTime), sizeof(float));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_fCurrentFrameTime), sizeof(float));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_bWrapping), sizeof(bool));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_bEnteringWrapping), sizeof(bool));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_wrappingEntrancePos), sizeof(sf::Vector2u));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_wrappingExitPos), sizeof(sf::Vector2u));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_wrappingEntranceDir), sizeof(sf::Vector2i));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_wrappingExitDir), sizeof(sf::Vector2i));
}

void MovingEntity::loadFromFile(File& loadFile)
{
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_fOgSpeed), sizeof(float));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_fSpeed), sizeof(float));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_movementDir), sizeof(sf::Vector2i));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_mapPos), sizeof(sf::Vector2u));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_posOffset), sizeof(sf::Vector2f));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_ogMovementDir), sizeof(sf::Vector2i));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_ogMapPos), sizeof(sf::Vector2u));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_ogPosOffset), sizeof(sf::Vector2f));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_bCanMove), sizeof(bool));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_bPaused), sizeof(bool));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_iMaxAnimationFrames), sizeof(U32));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_iMaxAnimationFrames), sizeof(U32));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_fMaxFrameTime), sizeof(float));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_fCurrentFrameTime), sizeof(float));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_bWrapping), sizeof(bool));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_bEnteringWrapping), sizeof(bool));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_wrappingEntrancePos), sizeof(sf::Vector2u));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_wrappingExitPos), sizeof(sf::Vector2u));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_wrappingEntranceDir), sizeof(sf::Vector2i));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_wrappingExitDir), sizeof(sf::Vector2i));

  SPtr<Transform> transform = getTransform().lock();
  transform->setLocalPosition(
    static_cast<sf::Vector2f>(m_mapPos) + m_posOffset);
}

void MovingEntity::setCanMove(bool can)
{
  m_bCanMove = can;
}
void MovingEntity::setPaused(bool paused)
{
  m_bPaused = paused;
}

void MovingEntity::move(U8 options)
{
  m_posOffset += sf::Vector2f(m_movementDir) * m_fSpeed *
                 gl::Time::instance().deltaTime();

  if (m_bWrapping) {
    SPtr<GameMap> map = m_pGameMap.lock();
    if (m_bEnteringWrapping) {
      if (m_wrappingEntranceDir.y == -1 && m_posOffset.y <= -map->m_fMaxWrappingDistance ||
          m_wrappingEntranceDir.x ==  1 && m_posOffset.x >=  map->m_fMaxWrappingDistance ||
          m_wrappingEntranceDir.y ==  1 && m_posOffset.y >=  map->m_fMaxWrappingDistance ||
          m_wrappingEntranceDir.x == -1 && m_posOffset.x <= -map->m_fMaxWrappingDistance) {
        m_mapPos = m_wrappingExitPos;
        m_bEnteringWrapping = false;
        m_posOffset = -m_posOffset;
      }
      else if (m_wrappingEntranceDir.y == -1 && m_posOffset.y >=  1.0f ||
               m_wrappingEntranceDir.x ==  1 && m_posOffset.x <= -1.0f ||
               m_wrappingEntranceDir.y ==  1 && m_posOffset.y <= -1.0f ||
               m_wrappingEntranceDir.x == -1 && m_posOffset.x >=  1.0f) {
        m_bWrapping = false;
        std::cout << "Wrapping exit " << getActor().lock()->getName() << std::endl;
      }
    }
    else {
      if (m_wrappingExitDir.y == -1 && m_posOffset.y >=  map->m_fMaxWrappingDistance ||
          m_wrappingExitDir.x ==  1 && m_posOffset.x <= -map->m_fMaxWrappingDistance ||
          m_wrappingExitDir.y ==  1 && m_posOffset.y <= -map->m_fMaxWrappingDistance ||
          m_wrappingExitDir.x == -1 && m_posOffset.x >=  map->m_fMaxWrappingDistance) {
            m_mapPos = m_wrappingEntrancePos;
            m_bEnteringWrapping = true;
            m_posOffset = -m_posOffset;
      }
      else if (m_wrappingExitDir.y == -1 && m_posOffset.y <= -1.0f ||
               m_wrappingExitDir.x ==  1 && m_posOffset.x >=  1.0f ||
               m_wrappingExitDir.y ==  1 && m_posOffset.y >=  1.0f ||
               m_wrappingExitDir.x == -1 && m_posOffset.x <= -1.0f) {
        m_bWrapping = false;
        std::cout << "Wrapping exit " << getActor().lock()->getName() << std::endl;
      }
    }
  }

  if (m_bWrapping) return;

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
  m_fCurrentFrameTime += gl::Time::instance().deltaTime();
  if (m_fCurrentFrameTime >= m_fMaxFrameTime) {
    m_fCurrentFrameTime -= m_fMaxFrameTime;
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
  if (m_bPaused) return;

  if (m_pGameMap.expired()) return;

  SPtr<GameMap> map = m_pGameMap.lock();
  if (m_bCanMove) {
    //std::cout << "GettingMovementOption " << getActor().lock()->getName() << std::endl;
    U8 options = map->getMovementOptions(m_mapPos);
    //std::cout << "GotMovementOption " << getActor().lock()->getName() << std::endl;

    move(options);

    if (!m_bWrapping) {
      GameMap::WrapInfo wrapInfo = {};
      m_bWrapping = map->isWrapping(m_mapPos, wrapInfo);
      if (m_bWrapping) {
        std::cout << "Wrapping enter " << getActor().lock()->getName() << std::endl;
        m_bEnteringWrapping = true;
        m_wrappingEntrancePos = wrapInfo.entrancePos;
        m_wrappingExitPos = wrapInfo.exitPos;
        m_wrappingEntranceDir = wrapInfo.entranceDir;
        m_wrappingExitDir = wrapInfo.exitDir;
      }
    }

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
