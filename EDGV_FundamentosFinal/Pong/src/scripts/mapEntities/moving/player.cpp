#include "scripts/mapEntities/moving/player.h"

#include <platform/math.h>

#include <tools/time.h>
#include <tools/textureManager.h>
#include <tools/input.h>

#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/shapeRect.h>
#include <scene/components/render.h>
#include <scene/components/rectCollider.h>

#include "scripts/gameManager.h"
#include "scripts/gameMap.h"

void Player::init(WPtr<GameMap> map, float speed,
                  FuntionPtr<void> eatablePhantoms, FuntionPtr<void> died,
                  FuntionPtr<void> finishedDying)
{
  m_pGameMap = map;
  m_fOgSpeed = speed;
  m_fSpeed = m_fOgSpeed;

  m_fpEatablePhantoms = eatablePhantoms;
  m_fpDied = died;
  m_fpFinishedDying = finishedDying;

  GameMap::SpawnInfo playerSpawnInfo = m_pGameMap.lock()->getPlayerSpawnInfo();

  m_ogMapPos = playerSpawnInfo.mapPos;
  m_ogMovementDir = playerSpawnInfo.initialDir;
  m_ogPosOffset = playerSpawnInfo.posOffset;

  m_mapPos = m_ogMapPos;
  m_movementDir = m_ogMovementDir;
  m_posOffset = m_ogPosOffset;

  m_bCollectedCoin = false;
  m_bCollectedCoinStep = false;
  m_bDead = false;

  m_iMaxAnimationFrames = 3;

  //std::cout << "initial m_posOffset(" << m_posOffset.x << ", " << m_posOffset.y << ")" << std::endl;
  //std::cout << "initial m_mapPos(" << m_mapPos.x << ", " << m_mapPos.y << ")" << std::endl;
  //std::cout << "initial m_movementDir(" << m_movementDir.x << ", " << m_movementDir.y << ")" << std::endl;

  SPtr<Actor> actor = getActor().lock();
  actor->addComponent<ShapeRect>();
  m_pRenderComp = actor->addComponent<Render>().lock();
  m_pRenderComp->m_iRenderIndex = 2;
  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("player_l2");
  SPtr<Transform> transform = getTransform().lock();
  transform->setLocalPosition(static_cast<sf::Vector2f>(m_mapPos));
  transform->setLocalScale({2.0f, 2.0f});
  SPtr<RectCollider> rectCollider = actor->addComponent<RectCollider>().lock();
  rectCollider->scale = {0.4f, 0.4f};
}


void Player::collectedCoin()
{
  std::cout << "collectedCoin" << std::endl;

  m_bCollectedCoin = true;
  m_bCollectedCoinStep = true;

  m_fSpeed = m_fOgSpeed * 0.85f;
}

void Player::collectedPowerCoin()
{
  std::cout << "collectedPowerCoin" << std::endl;

  if (m_fpEatablePhantoms == nullptr) return;
  m_fpEatablePhantoms();
}

void Player::dead()
{
  std::cout << "dead" << std::endl;

  m_bDead = true;
  m_bCanMove = false;

  m_iCurrentAnimationFrame = 0;
  m_iCurrentFrameTime = 0.0f;
  m_iMaxAnimationFrames = 12;

  m_fTimeToStartDeadAnimation = 0.0f;
  m_fTimeToFinishDeadAnimation = 0.0f;

  if (m_fpDied == nullptr) return;
  m_fpDied();
}

void Player::eatPhantom()
{
  std::cout << "eatPhantom" << std::endl;
}

void Player::reset()
{
  m_fSpeed = m_fOgSpeed;

  m_mapPos = m_ogMapPos;
  m_movementDir = m_ogMovementDir;
  m_posOffset = m_ogPosOffset;

  m_bCollectedCoin = false;
  m_bCollectedCoinStep = false;
  m_bDead = false;

  m_iMaxAnimationFrames = 3;

  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("player_l2");
}


U8 Player::getMovementDecision(const sf::Vector2i& dir, U8 options)
{
  bool canMoveUp = Math::hasFlag(options, eMOVEMENT_DIR::kUp);
  bool canMoveRight = Math::hasFlag(options, eMOVEMENT_DIR::kRight);
  bool canMoveDown = Math::hasFlag(options, eMOVEMENT_DIR::kDown);
  bool canMoveLeft = Math::hasFlag(options, eMOVEMENT_DIR::kLeft);
  
  bool pressingUp = gl::Input::getKey(sf::Keyboard::Key::W);
  bool pressingRight = gl::Input::getKey(sf::Keyboard::Key::D);
  bool pressingDown = gl::Input::getKey(sf::Keyboard::Key::S);
  bool pressingLeft = gl::Input::getKey(sf::Keyboard::Key::A);

  if (dir.y == -1) {
    if (canMoveRight && pressingRight) {
      return eMOVEMENT_DIR::kRight;
    }
    else if (canMoveDown && pressingDown) {
      return eMOVEMENT_DIR::kDown;
    }
    else if (canMoveLeft && pressingLeft) {
      return eMOVEMENT_DIR::kLeft;
    }
  }
  else if (dir.x == 1) {
    if (canMoveUp && pressingUp) {
      return eMOVEMENT_DIR::kUp;
    }
    else if (canMoveDown && pressingDown) {
      return eMOVEMENT_DIR::kDown;
    }
    else if (canMoveLeft && pressingLeft) {
      return eMOVEMENT_DIR::kLeft;
    }
  }
  else if (dir.y == 1) {
    if (canMoveUp && pressingUp) {
      return eMOVEMENT_DIR::kUp;
    }
    else if (canMoveRight && pressingRight) {
      return eMOVEMENT_DIR::kRight;
    }
    else if (canMoveLeft && pressingLeft) {
      return eMOVEMENT_DIR::kLeft;
    }
  }
  else if (dir.x == -1) {
    if (canMoveUp && pressingUp) {
      return eMOVEMENT_DIR::kUp;
    }
    else if (canMoveRight && pressingRight) {
      return eMOVEMENT_DIR::kRight;
    }
    else if (canMoveDown && pressingDown) {
      return eMOVEMENT_DIR::kDown;
    }
  }

  return 0;
}

void Player::setAnimationFrame(const sf::Vector2i& dir, U8 frame)
{
  if (m_bDead) {
    String animationName = "player_dead";
  
    m_pRenderComp->m_material.m_pTexture =
    gl::TextureManager::instance().getTexture(
     animationName + std::to_string(frame)
   );
  }
  else {
    MovingEntity::setAnimationFrame(dir, frame);
  }
}

void Player::move(U8 options)
{
  if (!m_bDead) {
    MovingEntity::move(options);
  }
}

void Player::update()
{
  if (m_bDead) {
    if (m_fTimeToStartDeadAnimation >= m_fMaxTimeToStartDeadAnimation) {
      if (m_iCurrentAnimationFrame == 11) {
        m_fTimeToFinishDeadAnimation += gl::Time::instance().deltaTime();

        if (m_fTimeToFinishDeadAnimation >= m_fMaxTimeToFinishDeadAnimation) {
          if (m_fpFinishedDying != nullptr) m_fpFinishedDying();

          m_bDead = false;
        }
      }
      else {
        moveAnimation();
      }
    }
    else {
      m_fTimeToStartDeadAnimation += gl::Time::instance().deltaTime();
    }
  }
  else {
    MovingEntity::update();

    if (m_lastPos != m_mapPos) {
      m_lastPos = m_mapPos;
  
      if (m_bCollectedCoin) {
        if (m_bCollectedCoinStep){
          m_bCollectedCoinStep = false;
        }
        else {
          m_bCollectedCoin = false;
          m_fSpeed = m_fOgSpeed;
        }
      }
    }
  }
}