#include "scripts/mapEntities/collectables/coin.h"

#include "scripts/mapEntities/moving/player.h"

void Coin::onCollected(WPtr<Player> player)
{
  player.lock()->collectedCoin();
}