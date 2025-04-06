#include "scripts/mapEntities/collectables/powerCoin.h"

#include "scripts/mapEntities/moving/player.h"

void PowerCoin::onCollected(WPtr<Player> player)
{
  player.lock()->collectedPowerCoin();
}