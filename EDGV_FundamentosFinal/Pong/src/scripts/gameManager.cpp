#include "scripts/gameManager.h"

#include <tools/textureManager.h>

#include <scene/scene.h>
#include <scene/actor.h>
#include <scene/components/transform.h>

#include <scene/components/canvas.h>
#include <scene/components/render.h>
#include <scene/components/shapeRect.h>

#include "scripts/statesPattern/stateMachine.h"

#include "scripts/gameMap.h"
#include "scripts/mapBuilder.h"
#include "scripts/mapEntities/moving/player.h"
#include "scripts/mapEntities/moving/phantoms/blinky.h"
#include "scripts/mapEntities/moving/phantoms/pinky.h"
#include "scripts/mapEntities/moving/phantoms/inky.h"
#include "scripts/mapEntities/moving/phantoms/clyde.h"

void GameManager::start()
{
  SPtr<Actor> stateMachine = getScene().lock()->addActor("stateMachine").lock();
  SPtr<StateMachine> stateMachineComp = stateMachine->addComponent<StateMachine>().lock();
  stateMachineComp->init(shared_from_this());

  SPtr<Actor> gameMap = getScene().lock()->addActor("gameMap").lock();
  m_pGameMap = gameMap->addComponent<GameMap>().lock();
  SPtr<Transform> gameMapTrans = gameMap->getTransform().lock();
  gameMapTrans->setLocalPosition({0.5f, 0.5f});

  SPtr<Actor> mapBuilder = getScene().lock()->addActor("mapBuilder").lock();
  SPtr<MapBuilder> mapBuilderComp = mapBuilder->addComponent<MapBuilder>().lock();
  mapBuilderComp->init(L"../Pong/resources/baseMap.map", m_pGameMap);

  m_pGameMap->init();

  SPtr<Actor> player = getScene().lock()->addActor("player").lock();
  m_pPlayer = player->addComponent<Player>().lock();
  player->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pPlayer->init(m_pGameMap, 6.0f, [this](){ this->eatablePhantoms(); },
                  [this](){ this->playerDied(); },
                  [this](){ this->resetGame(); });
  
  SPtr<Actor> blinky = getScene().lock()->addActor("blinky").lock();
  m_pBlinky = blinky->addComponent<Blinky>().lock();
  blinky->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pBlinky->init(m_pGameMap, 5.5f, 0);
  
  SPtr<Actor> pinky = getScene().lock()->addActor("pinky").lock();
  m_pPinky = pinky->addComponent<Pinky>().lock();
  pinky->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pPinky->init(m_pGameMap, 5.5f, 0);
  
  SPtr<Actor> inky = getScene().lock()->addActor("inky").lock();
  m_pInky = inky->addComponent<Inky>().lock();
  inky->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pInky->init(m_pGameMap, 5.5f, 0);
  
  SPtr<Actor> clyde = getScene().lock()->addActor("clyde").lock();
  m_pClyde = clyde->addComponent<Clyde>().lock();
  clyde->getTransform().lock()->attachTo(gameMap->getTransform());
  m_pClyde->init(m_pGameMap, 5.5f, 0);

  m_pGameMap->getActor().lock()->setActive(false);


  // {672, 888}
  SPtr<Actor> canvas = getScene().lock()->addActor("canvas").lock();
  canvas->addComponent<Canvas>();

  SPtr<Actor> mainMenu = getScene().lock()->addActor("mainMenu").lock();
  WPtr<Transform> mainMenuT = mainMenu->getTransform();

  SPtr<Actor> mainMenuTitle = getScene().lock()->addActor("title").lock();
  SPtr<Transform> mainMenuTitleT = mainMenuTitle->getTransform().lock();
  mainMenuTitleT->attachTo(mainMenuT);

  SPtr<Actor> mainMenuNewGameBtn = getScene().lock()->addActor("newGameBtn").lock();
  SPtr<Transform> mainMenuNewGameBtnT = mainMenuNewGameBtn->getTransform().lock();
  mainMenuNewGameBtnT->attachTo(mainMenuT);

  SPtr<Actor> mainMenuLoadGameBtn = getScene().lock()->addActor("loadGameBtn").lock();
  SPtr<Transform> mainMenuLoadGameBtnT = mainMenuLoadGameBtn->getTransform().lock();
  mainMenuLoadGameBtnT->attachTo(mainMenuT);

  SPtr<Actor> mainMenuExitBtn = getScene().lock()->addActor("exitBtn").lock();
  SPtr<Transform> mainMenuExitBtnT = mainMenuExitBtn->getTransform().lock();
  mainMenuExitBtnT->attachTo(mainMenuT);
}

void GameManager::prepareGame()
{
  m_pGameMap->getActor().lock()->setActive(true);
}
void GameManager::startPlaying()
{
  m_pPlayer->setCanMove(true);
  m_pBlinky->setCanMove(true);
  m_pPinky->setCanMove(true);
  m_pInky->setCanMove(true);
  m_pClyde->setCanMove(true);
}
void GameManager::pause()
{

}

void GameManager::eatablePhantoms()
{
  m_pBlinky->makeEatable(8.0f);
  m_pPinky->makeEatable(8.0f);
  m_pInky->makeEatable(8.0f);
  m_pClyde->makeEatable(8.0f);
}

void GameManager::playerDied()
{
  m_pBlinky->getActor().lock()->setActive(false);
  m_pBlinky->setCanMove(false);
  m_pPinky->getActor().lock()->setActive(false);
  m_pPinky->setCanMove(false);
  m_pInky->getActor().lock()->setActive(false);
  m_pInky->setCanMove(false);
  m_pClyde->getActor().lock()->setActive(false);
  m_pClyde->setCanMove(false);
}

void GameManager::resetGame()
{
  m_pPlayer->reset();
  m_pBlinky->reset();
  m_pPinky->reset();
  m_pInky->reset();
  m_pClyde->reset();

  m_pPlayer->setCanMove(true);
  m_pBlinky->getActor().lock()->setActive(true);
  m_pBlinky->setCanMove(true);
  m_pPinky->getActor().lock()->setActive(true);
  m_pPinky->setCanMove(true);
  m_pInky->getActor().lock()->setActive(true);
  m_pInky->setCanMove(true);
  m_pClyde->getActor().lock()->setActive(true);
  m_pClyde->setCanMove(true);
}