#include "scripts/mapEntities/coin.h"

#include <platform/memoryManager.hpp>
#include <scene/actor.h>
#include <scene/scene.h>
#include <scene/components/rectCollider.h>
#include <resources/collider.h>

#include "scripts/mapEntities/moving/player.h"

void Coin::start()
{
  SPtr<RectCollider> rectCollider = getActor().lock()->addComponent<RectCollider>().lock();
  rectCollider->scale = {0.8f, 0.8f};
}

void Coin::onCollisionEnter(CollisionInfo info)
{
  WPtr<Player> player = info.otherActor.lock()->getComponent<Player>();
  if (!player.expired()) {
    player.lock()->collectedCoin();
    //getScene().lock()->destroyActor(getActor());
    getActor().lock()->setActive(false);
    //std::cout << "End collision" << std::endl;
  }
}