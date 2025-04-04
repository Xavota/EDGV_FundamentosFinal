#include "scripts/player.h"

#include <platform/math.h>

#include <tools/time.h>
#include <tools/textureManager.h>
#include <tools/input.h>

#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/shapeRect.h>
#include <scene/components/render.h>
#include <scene/components/rectCollider.h>

#include "scripts/gameMap.h"

void Player::init(WPtr<GameMap> map, float speed)
{
  m_pGameMap = map;
  m_fSpeed = speed;

  GameMap::SpawnInfo playerSpawnInfo = m_pGameMap.lock()->getPlayerSpawnInfo();

  m_mapPos = playerSpawnInfo.mapPos;
  m_movementDir = playerSpawnInfo.initialDir;
  m_posOffset = playerSpawnInfo.posOffset;
  //std::cout << "initial m_posOffset(" << m_posOffset.x << ", " << m_posOffset.y << ")" << std::endl;
  //std::cout << "initial m_mapPos(" << m_mapPos.x << ", " << m_mapPos.y << ")" << std::endl;
  //std::cout << "initial m_movementDir(" << m_movementDir.x << ", " << m_movementDir.y << ")" << std::endl;

  SPtr<Actor> actor = getActor().lock();
  actor->addComponent<ShapeRect>();
  m_pRenderComp = actor->addComponent<Render>().lock();
  m_pRenderComp->m_iRenderIndex = 2;
  m_pRenderComp->m_material.m_pTexture = gl::TextureManager::instance().getTexture("player_l2");
  SPtr<Transform> transform = getTransform().lock();
  transform->setLocalPosition(static_cast<sf::Vector2f>(m_mapPos));
  transform->setLocalScale({2.0f, 2.0f});
  SPtr<RectCollider> rectCollider = actor->addComponent<RectCollider>().lock();
  rectCollider->scale = {0.4f, 0.4f};
}

void Player::collectedCoin()
{
}

void Player::collectedPowerCoin()
{

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