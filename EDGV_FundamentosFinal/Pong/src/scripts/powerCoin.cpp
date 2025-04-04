#include "scripts/powerCoin.h"

#include <platform/memoryManager.hpp>
#include <scene/actor.h>
#include <scene/scene.h>
#include <scene/components/rectCollider.h>
#include <resources/collider.h>

#include "scripts/player.h"

void PowerCoin::start()
{
  getActor().lock()->addComponent<RectCollider>();
}

void PowerCoin::onCollisionEnter(CollisionInfo info)
{
  WPtr<Player> player = info.otherActor.lock()->getComponent<Player>();
  if (!player.expired()) {
    player.lock()->collectedPowerCoin();
    //getScene().lock()->destroyActor(getActor());
    getActor().lock()->setActive(false);
  }
}