#include "scripts/mapEntities/moving/phantom.h"

#include <platform/math.h>
#include <platform/iofile.h>

#include <tools/time.h>
#include <tools/textureManager.h>

#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/shapeRect.h>
#include <scene/components/render.h>
#include <scene/components/rectCollider.h>

#include "scripts/gameMap.h"
#include "scripts/mapEntities/moving/player.h"

void Phantom::saveToFile(File& saveFile)
{
  MovingEntity::saveToFile(saveFile);

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_lastPos), sizeof(sf::Vector2u));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_bIsEatable), sizeof(bool));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_bIsBeenEaten), sizeof(bool));

  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_fEatableMaxTime), sizeof(float));
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_fEatableTime), sizeof(float));
}

void Phantom::loadFromFile(File& loadFile)
{
  MovingEntity::loadFromFile(loadFile);

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_lastPos), sizeof(sf::Vector2u));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_bIsEatable), sizeof(bool));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_bIsBeenEaten), sizeof(bool));

  loadFile.readBytes(reinterpret_cast<Byte*>(&m_fEatableMaxTime), sizeof(float));
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_fEatableTime), sizeof(float));

  setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);
}

void Phantom::init(WPtr<GameMap> map, float speed, U8 phantomIndex)
{
  m_pGameMap = map;
  m_fOgSpeed = speed;
  m_fSpeed = m_fOgSpeed;

  m_iMaxAnimationFrames = 2;

  GameMap::SpawnInfo phantomSpawnInfo = m_pGameMap.lock()->getPhantomSpawnInfo(phantomIndex);

  m_ogMapPos = phantomSpawnInfo.mapPos;
  m_ogMovementDir = phantomSpawnInfo.initialDir;
  m_ogPosOffset = phantomSpawnInfo.posOffset;

  m_mapPos = m_ogMapPos;
  m_movementDir = m_ogMovementDir;
  m_posOffset = m_ogPosOffset;

  //std::cout << "initial m_posOffset(" << m_posOffset.x << ", " << m_posOffset.y << ")" << std::endl;
  //std::cout << "initial m_mapPos(" << m_mapPos.x << ", " << m_mapPos.y << ")" << std::endl;
  //std::cout << "initial m_movementDir(" << m_movementDir.x << ", " << m_movementDir.y << ")" << std::endl;

  SPtr<Actor> actor = getActor().lock();
  actor->addComponent<ShapeRect>();
  m_pRenderComp = actor->addComponent<Render>().lock();
  m_pRenderComp->m_iRenderIndex = 3;
  SPtr<Transform> transform = getTransform().lock();
  transform->setLocalPosition(static_cast<sf::Vector2f>(m_mapPos) + m_posOffset);
  transform->setLocalScale({2.0f, 2.0f});
  SPtr<RectCollider> rectCollider = actor->addComponent<RectCollider>().lock();
  rectCollider->scale = {0.4f, 0.4f};
}

void Phantom::makeEatable(float time)
{
  if (m_bIsBeenEaten) return;

  m_bIsEatable = true;
  m_lastPos = m_lastPos + static_cast<sf::Vector2u>(m_movementDir);
  m_movementDir = -m_movementDir;

  m_fEatableMaxTime = time;
  m_fEatableTime = 0.0f;

  m_fSpeed = m_fOgSpeed * 0.65f;
}

void Phantom::makeNotEatable()
{
  if (!m_bIsEatable) return;

  m_bIsEatable = false;
  m_fSpeed = m_fOgSpeed;
}

void Phantom::eaten()
{
  if (m_bIsBeenEaten) return;

  makeNotEatable();

  m_bIsBeenEaten = true;
  m_fSpeed = m_fOgSpeed * 1.1f;
}

void Phantom::reset()
{
  makeNotEatable();

  m_mapPos = m_ogMapPos;
  m_movementDir = m_ogMovementDir;
  m_posOffset = m_ogPosOffset;

  m_bIsBeenEaten = false;

  SPtr<Transform> transform = getTransform().lock();
  transform->setLocalPosition(static_cast<sf::Vector2f>(m_mapPos) + m_posOffset);
}

U8 Phantom::getMovementDecision(const sf::Vector2i& dir, U8 options)
{
  if (m_bWrapping || m_bIsBeenEaten || options == 0 || m_lastPos == m_mapPos) return 0;

  m_lastPos = m_mapPos;

  bool canMoveUp = Math::hasFlag(options, eMOVEMENT_DIR::kUp);
  bool canMoveRight = Math::hasFlag(options, eMOVEMENT_DIR::kRight);
  bool canMoveDown = Math::hasFlag(options, eMOVEMENT_DIR::kDown);
  bool canMoveLeft = Math::hasFlag(options, eMOVEMENT_DIR::kLeft);

  std::random_device rd;
  std::mt19937 gen(rd());

  Vector<U8> optionsList = {};
  if (dir.y == -1) {
    if (canMoveUp) {
      optionsList.push_back(0);
      optionsList.push_back(0);
    }
    if (canMoveRight) {
      optionsList.push_back(eMOVEMENT_DIR::kRight);
    }
    if (canMoveLeft) {
      optionsList.push_back(eMOVEMENT_DIR::kLeft);
    }

    if (optionsList.empty()) return eMOVEMENT_DIR::kDown;
  }
  else if (dir.x == 1) {
    if (canMoveUp) {
      optionsList.push_back(eMOVEMENT_DIR::kUp);
    }
    if (canMoveRight) {
      optionsList.push_back(0);
      optionsList.push_back(0);
    }
    if (canMoveDown) {
      optionsList.push_back(eMOVEMENT_DIR::kDown);
    }

    if (optionsList.empty()) return eMOVEMENT_DIR::kLeft;
  }
  else if (dir.y == 1) {
    if (canMoveRight) {
      optionsList.push_back(eMOVEMENT_DIR::kRight);
    }
    if (canMoveDown) {
      optionsList.push_back(0);
      optionsList.push_back(0);
    }
    if (canMoveLeft) {
      optionsList.push_back(eMOVEMENT_DIR::kLeft);
    }

    if (optionsList.empty()) return eMOVEMENT_DIR::kUp;
  }
  else if (dir.x == -1) {
    if (canMoveUp) {
      optionsList.push_back(eMOVEMENT_DIR::kUp);
    }
    if (canMoveDown) {
      optionsList.push_back(eMOVEMENT_DIR::kDown);
    }
    if (canMoveLeft) {
      optionsList.push_back(0);
      optionsList.push_back(0);
    }

    if (optionsList.empty()) return eMOVEMENT_DIR::kRight;
  }

  if (optionsList.empty()) return 0;

  std::uniform_int_distribution<> distrib(0, optionsList.size() - 1);

  U32 option = distrib(gen);

  auto r = optionsList[option];

  //std::cout << "m_mapPos (" << m_mapPos.x << "," << m_mapPos.y << ")" << std::endl;
  //std::cout << "options: " << I32(options) << std::endl;
  //std::cout << "decision: " << I32(r) << std::endl;

  return r;
}

void Phantom::setAnimationFrame(const sf::Vector2i& dir, U8 frame)
{
  if (m_bIsEatable) {
    String animationName = "phantoms_eatable";
    if (m_fEatableMaxTime - m_fEatableTime <= 2.0f) {
      I32 frameType = static_cast<I32>(std::ceil((m_fEatableMaxTime - m_fEatableTime) * 4.8f));

      if (frameType % 2 == 1) {
        animationName += "_finish";
      }
    }
  
    m_pRenderComp->m_material.m_pTexture =
    gl::TextureManager::instance().getTexture(
     animationName + std::to_string(frame)
   );
  }
  else if (m_bIsBeenEaten) {
    String animationName = "phantoms_eyes";
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
      animationName + renderFrameLetter
    );
  }
  else {
    MovingEntity::setAnimationFrame(dir, frame);
  }
}

void Phantom::move(U8 options)
{
  if (m_bIsBeenEaten) {
    std::cout << "move been eaten" << std::endl;

    setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);

    SPtr<Transform> transform = getTransform().lock();
    sf::Vector2f currentPos = transform->getLocalPosition();
    std::cout << "currentPos(" << currentPos.x << ", " << currentPos.y << ")" << std::endl;
    sf::Vector2f returnPos = static_cast<sf::Vector2f>(m_ogMapPos) + m_ogPosOffset;
    std::cout << "returnPos(" << returnPos.x << ", " << returnPos.y << ")" << std::endl;
    sf::Vector2f returnDir = returnPos - currentPos;
    std::cout << "returnDir(" << returnDir.x << ", " << returnDir.y << ")" << std::endl;
    std::cout << "returnDir.length(" << returnDir.length() << ")" << std::endl;
    if (returnDir.length() <= 0.1f) {
      reset();
      return;
    }

    sf::Vector2f returnDirN = returnDir.normalized();

    sf::Vector2f newPos = currentPos +
    returnDirN * m_fSpeed * gl::Time::instance().deltaTime();
    std::cout << "newPos(" << newPos.x << ", " << newPos.y << ")" << std::endl;

    m_posOffset = newPos - static_cast<sf::Vector2f>(m_mapPos);

    //transform->setLocalPosition(newPos);
  }
  else {
    MovingEntity::move(options);
  }
}

void Phantom::onCollisionEnter(CollisionInfo info)
{
  WPtr<Player> player = info.otherActor.lock()->getComponent<Player>();
  if (!player.expired()) {
    if (!m_bIsBeenEaten) {
      if (m_bIsEatable) {
        player.lock()->eatPhantom();
        eaten();
      }
      else player.lock()->dead();
    }
    
    //getScene().lock()->destroyActor(getActor());
    //getActor().lock()->setActive(false);
    //std::cout << "End collision" << std::endl;
  }
}

void Phantom::update()
{
  MovingEntity::update();

  if (m_bPaused) return;

  if (m_bIsEatable && m_bCanMove) {
    m_fEatableTime += gl::Time::instance().deltaTime();

    setAnimationFrame(m_movementDir, m_iCurrentAnimationFrame);

    if (m_fEatableTime >= m_fEatableMaxTime) { 
      makeNotEatable();
    }
  }
}