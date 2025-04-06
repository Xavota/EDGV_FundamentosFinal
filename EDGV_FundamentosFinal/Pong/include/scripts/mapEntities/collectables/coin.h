#pragma once

#include "scripts/mapEntities/collectable.h"

class Coin : public Collectable
{
 public:
  /**
   * @brief Default constructor.
   */
  Coin() = default;
  /**
   * @brief Default destructor.
   */
  ~Coin() = default;

 protected:
  void onCollected(WPtr<Player> player) override;
};