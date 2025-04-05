#pragma once

#include <scene/components/script.h>

class PowerCoin : public Script
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

 private:
  void start() override;

  void onCollisionEnter(CollisionInfo info) override;
};