#include "scripts/gameManager.h"

#include <tools/textureManager.h>

#include <scene/scene.h>
#include <scene/actor.h>
#include <scene/components/transform.h>

#include "scripts/gameMap.h"
#include "scripts/mapBuilder.h"
#include "scripts/player.h"
#include "scripts/blinky.h"
#include "scripts/pinky.h"
#include "scripts/inky.h"
#include "scripts/clyde.h"

void GameManager::start()
{
  SPtr<Actor> gameMap = getScene().lock()->addActor("gameMap").lock();
  SPtr<GameMap> gameMapComp = gameMap->addComponent<GameMap>().lock();
  SPtr<Transform> gameMapTrans = gameMap->getTransform().lock();
  gameMapTrans->setLocalPosition({0.5f, 0.5f});

  SPtr<Actor> mapBuilder = getScene().lock()->addActor("mapBuilder").lock();
  SPtr<MapBuilder> mapBuilderComp = mapBuilder->addComponent<MapBuilder>().lock();
  mapBuilderComp->init(L"../Pong/resources/baseMap.map", gameMapComp);

  gameMapComp->init();

  SPtr<Actor> player = getScene().lock()->addActor("player").lock();
  SPtr<Player> playerComp = player->addComponent<Player>().lock();
  player->getTransform().lock()->attachTo(gameMap->getTransform());
  playerComp->init(gameMapComp, 6.0f);
  
  SPtr<Actor> blinky = getScene().lock()->addActor("blinky").lock();
  SPtr<Blinky> blinkyComp = blinky->addComponent<Blinky>().lock();
  blinky->getTransform().lock()->attachTo(gameMap->getTransform());
  blinkyComp->init(gameMapComp, 4.5f, 0);
  
  SPtr<Actor> pinky = getScene().lock()->addActor("pinky").lock();
  SPtr<Pinky> pinkyComp = pinky->addComponent<Pinky>().lock();
  pinky->getTransform().lock()->attachTo(gameMap->getTransform());
  pinkyComp->init(gameMapComp, 4.5f, 0);
  
  SPtr<Actor> inky = getScene().lock()->addActor("inky").lock();
  SPtr<Inky> inkyComp = inky->addComponent<Inky>().lock();
  inky->getTransform().lock()->attachTo(gameMap->getTransform());
  inkyComp->init(gameMapComp, 4.5f, 0);
  
  SPtr<Actor> clyde = getScene().lock()->addActor("clyde").lock();
  SPtr<Clyde> clydeComp = clyde->addComponent<Clyde>().lock();
  clyde->getTransform().lock()->attachTo(gameMap->getTransform());
  clydeComp->init(gameMapComp, 4.5f, 0);

  playerComp->setCanMove(true);
  blinkyComp->setCanMove(true);
  pinkyComp->setCanMove(true);
  inkyComp->setCanMove(true);
  clydeComp->setCanMove(true);
}