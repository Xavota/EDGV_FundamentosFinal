#include "scripts/mapEntities/collectable.h"

#include <platform/memoryManager.hpp>
#include <scene/actor.h>
#include <scene/scene.h>
#include <scene/components/rectCollider.h>
#include <resources/collider.h>

#include "scripts/mapEntities/moving/player.h"

void Collectable::start()
{
  SPtr<RectCollider> rectCollider = getActor().lock()->addComponent<RectCollider>().lock();
  rectCollider->scale = {0.8f, 0.8f};
  //rectCollider->render = true;
}

void Collectable::onCollisionEnter(CollisionInfo info)
{
  WPtr<Player> player = info.otherActor.lock()->getComponent<Player>();
  if (!player.expired()) {
    onCollected(player);
    if (m_fpCollected != nullptr) m_fpCollected();
    //getActor().lock()->setActive(false);
    //getScene().lock()->destroyActor(getActor());
  }
}