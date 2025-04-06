#pragma once

#include "scripts/mapEntities/collectable.h"

class PowerCoin : public Collectable
{
 public:
  /**
   * @brief Default constructor.
   */
  PowerCoin() = default;
  /**
   * @brief Default destructor.
   */
  ~PowerCoin() = default;

 protected:
  void onCollected(WPtr<Player> player) override;
};