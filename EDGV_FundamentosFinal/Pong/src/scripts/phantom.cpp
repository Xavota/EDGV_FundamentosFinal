#include "scripts/phantom.h"

#include <platform/math.h>

#include <tools/time.h>
#include <tools/textureManager.h>

#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/shapeRect.h>
#include <scene/components/render.h>
#include <scene/components/rectCollider.h>

#include "scripts/gameMap.h"

void Phantom::init(WPtr<GameMap> map, float speed, U8 phantomIndex)
{
  m_pGameMap = map;
  m_fSpeed = speed;

  m_iMaxAnimationFrames = 2;

  GameMap::SpawnInfo phantomSpawnInfo = m_pGameMap.lock()->getPhantomSpawnInfo(phantomIndex);

  m_mapPos = phantomSpawnInfo.mapPos;
  m_movementDir = phantomSpawnInfo.initialDir;
  m_posOffset = phantomSpawnInfo.posOffset;
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

U8 Phantom::getMovementDecision(const sf::Vector2i& dir, U8 options)
{
  if (options == 0) return 0;

  if (m_lastPos == m_mapPos) return 0;

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
